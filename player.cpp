// player.cpp
#include "player.hpp"
#include "catan.hpp"
#include "board.hpp"
#include "land.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>   // For std::floor function

Player::Player(const std::string &name, int color) 
    : name(name), color(color), points(0), lands(19, false), developmentCards(5, 0), usedKnights(0) {
    resources = {{"wood", 0}, {"brick", 0}, {"wheat", 0}, {"ore", 0}, {"wool", 0}};
}

const std::string& Player::getName() const {
    return name;
}

int Player::getColor() const {
    return color;
}

bool Player::hasResources(const std::map<std::string, int> &cost) const {
    for (const auto &item : cost) {
        if (resources.at(item.first) < item.second) {
            return false;
        }
    }
    return true;
}

void Player::deductResources(const std::map<std::string, int> &cost) {
    for (const auto &item : cost) {
        resources[item.first] -= item.second;
    }
}

void Land::initialOccupy(Catan &game) {
    for (int i = 0; i < 2; ++i) { // Repeat the process twice
        int landIndex, spotIndex;
        std::vector<std::pair<int, int>> adjacentRoads;

        // Loop until a valid settlement is chosen
        do {
            std::cout << "Player " << color << ", choose a land index (0-18): ";
            std::cin >> landIndex;
            std::cout << "Choose a settlement index (0-5): ";
            std::cin >> spotIndex;

            if (landIndex < 0 || landIndex >= 19 || spotIndex < 0 || spotIndex >= 6) {
                std::cout << "Invalid indexes. Try again." << std::endl;
                continue;
            }

            Land &land = game.getBoard().getLand(landIndex);
            adjacentRoads = land.isValidSettlementInitial(spotIndex, color, game);
            if (adjacentRoads.empty()) {
                std::cout << "Invalid settlement spot. Try again." << std::endl;
            }
        } while (adjacentRoads.empty());

        // Settle on the chosen spot
        Land &land = game.getBoard().getLand(landIndex);
        land.settlements[spotIndex] = color;
        land.occupyNeighborsSpot(spotIndex, color, game);

        // Update player's lands
        Player &player = game.getPlayer(color);
        player.lands[landIndex] = true;

        // Prompt user to choose one of the adjacent roads until valid choice is made
        int roadChoice;
        do {
            std::cout << "Choose one of the adjacent roads:" << std::endl;
            for (size_t j = 0; j < adjacentRoads.size(); ++j) {
                std::cout << j << ": Land index " << adjacentRoads[j].first << ", Road index " << adjacentRoads[j].second << std::endl;
            }
            std::cin >> roadChoice;

            if (roadChoice < 0 || roadChoice >= adjacentRoads.size()) {
                std::cout << "Invalid road choice. Try again." << std::endl;
            } else {
                int chosenLandIndex = adjacentRoads[roadChoice].first;
                int chosenRoadIndex = adjacentRoads[roadChoice].second;

                Land &chosenLand = game.getBoard().getLand(chosenLandIndex);
                chosenLand.roads[chosenRoadIndex] = color;
                chosenLand.occupyNeighborsRoad(chosenRoadIndex, color, game);

                // Update player's lands
                player.lands[chosenLandIndex] = true;
                break; // Exit the loop when a valid choice is made
            }
        } while (true);
    }
}

void Player::placeSettlement(int landIndex, int spotIndex, Catan &game) {
    static const std::map<std::string, int> settlementCost = {{"brick", 1}, {"wood", 1}, {"wool", 1}, {"wheat", 1}};
    
    if (landIndex < 0 || landIndex >= 19 || spotIndex < 0 || spotIndex >= 6) {
        std::cout << "Invalid land index or spot index. Cancelling the placement." << std::endl;
        return;
    }

    if (!hasResources(settlementCost)) {
        std::cout << "Not enough resources to place a settlement. Cancelling the purchase." << std::endl;
        return;
    }
    Land &land = game.getBoard().getLand(landIndex);
    if(land.occupySpot(spotIndex, color)){
        lands[landIndex] = true;
        points++;
        deductResources(settlementCost);
        std::cout << name << " placed a settlement on land " << landIndex << " at spot " << spotIndex << std::endl;
    }
}



void Player::placeRoad(int landIndex, int roadIndex, Catan &game) {
    static const std::map<std::string, int> roadCost = {{"brick", 1}, {"wood", 1}};

    if (landIndex < 0 || landIndex >= 19 || roadIndex < 0 || roadIndex >= 6) {
        std::cout << "Invalid land index or road index. Cancelling the placement." << std::endl;
        return;
    }

    if (!hasResources(roadCost)) {
        std::cout << "Not enough resources to place a road. Cancelling the purchase." << std::endl;
        return;
    }

    Land &land = game.getBoard().getLand(landIndex);
    if(land.occupyRoad(roadIndex, color)){
        deductResources(roadCost);
        lands[landIndex] = true;
        std::cout << name << " placed a road on land " << landIndex << " at road " << roadIndex << std::endl;
    }
}


void Player::upgradeSettlement(int landIndex, int spotIndex, Catan &game) {
    static const std::map<std::string, int> cityCost = {{"ore", 3}, {"wheat", 2}};

    if (landIndex < 0 || landIndex >= 19 || spotIndex < 0 || spotIndex >= 6) {
        std::cout << "Invalid land index or road index. Cancelling the placement." << std::endl;
        return;
    }

    if (!hasResources(cityCost)) {
        std::cout << "Not enough resources to upgrade to a city. Cancelling the upgrade." << std::endl;
        return;
    }

    Land &land = game.getBoard().getLand(landIndex);
    if (land.upgradeToCity(spotIndex, color, game)) {
        points++;
        deductResources(cityCost);
        std::cout << name << " upgraded a settlement to a city on land " << landIndex << " at spot " << spotIndex << std::endl;
    } else {
        std::cout << "Invalid city upgrade. Cancelling the upgrade." << std::endl;
    }
}


void Player::rollDice(Catan &game) {
    int diceRoll = rand() % 11 + 2; // Generates a random number from 2 to 12

    if (diceRoll == 7) {
        std::cout << "Dice rolled: " << diceRoll << std::endl;
        int robberPlacement;
        std::cout << "Choose a number between 0-18 to place the robber: ";
        std::cin >> robberPlacement;

        // Update the board's robber position
        game.getBoard().placeRobber(robberPlacement);

        // Iterate over each player to handle resource checking and discarding
        for (int playerId = 1; playerId <= 3; ++playerId) {
            Player &currentPlayer = game.getPlayer(playerId);

            // Count total resources
            int totalResources = 0;
            for (const auto &res : currentPlayer.resources) {
                totalResources += res.second;
            }

            // Check if player needs to discard
            int discardCount = std::floor(totalResources / 2.0);
            if (totalResources > 7) {
                std::cout << "Player " << currentPlayer.getName() << ", you need to discard " << discardCount << " cards." << std::endl;
                std::cout << "You currently have: ";
                for (const auto &res : currentPlayer.resources) {
                    std::cout << res.second << " " << res.first << ", ";
                }
                std::cout << std::endl;

                int discardedCount = 0;
                while (discardedCount < discardCount) {
                    std::string resourceName;
                    int amount;
                    std::cout << "Enter resource name and amount to discard (e.g., wool 2): ";
                    std::cin >> resourceName >> amount;

                    // Validate and process discard
                    std::map<std::string, int> cost = {{resourceName, amount}};
                    if (currentPlayer.hasResources(cost) && discardedCount + amount <= discardCount) {
                        currentPlayer.resources[resourceName] -= amount;
                        discardedCount += amount;
                        std::cout << "Reduced " << amount << " " << resourceName << std::endl;
                    } else {
                        std::cout << "Invalid request. Try again." << std::endl;
                    }
                }
            }
        }
        std::cout << "Players finished discarding." << std::endl;
    } else {
        // Non-7 dice roll logic
        std::cout << "Dice rolled: " << diceRoll << std::endl;

        // Iterate over all lands on the board
        for (int landIndex = 0; landIndex < 19; ++landIndex) {
            Land &land = game.getBoard().getLand(landIndex);
            if (land.getProbability() == diceRoll && game.getBoard().getRobberLocation() != landIndex) {
                // Check settlements and cities on this land
                for (int spotIndex = 0; spotIndex < 6; ++spotIndex) {
                    int ownerColor = land.cities[spotIndex]; // Check if there's a city first
                    if (ownerColor == 0) {
                        ownerColor = land.settlements[spotIndex]; // If no city, check for settlement
                    }
                    if (ownerColor != 0) {
                        // Add resources directly to the owner's resources
                        game.getPlayer(ownerColor).resources[land.getType()] += (land.cities[spotIndex] == 0) ? 1 : 2;
                    }
                }
            }
        }

        // Print collected resources for the current player
        std::cout << "Current player's resources:" << std::endl;
        for (int playerId = 1; playerId <= game.getNumPlayers(); ++playerId) {
            std::cout << "Player " << playerId << ": ";
            bool firstResource = true;
            for (const auto &res : game.getPlayer(playerId).resources) {
                if (!firstResource) {
                    std::cout << ", ";
                }
                std::cout << res.second << " " << res.first;
                firstResource = false;
            }
        }
        std::cout << std::endl;

    }
}



void Player::endTurn(Catan &game) {
    game.nextTurn();
}

void Player::trade(Player &other, const std::vector<std::string> &offer, const std::vector<int> &offerAmounts, const std::vector<std::string> &request, const std::vector<int> &requestAmounts) {
    // Check if the trade is valid for both players
    for (size_t i = 0; i < offer.size(); ++i) {
        if (resources[offer[i]] < offerAmounts[i]) {
            throw std::runtime_error("Trade not possible due to insufficient resources for offer.");
        }
    }
    for (size_t i = 0; i < request.size(); ++i) {
        if (other.resources[request[i]] < requestAmounts[i]) {
            throw std::runtime_error("Trade not possible due to insufficient resources for request.");
        }
    }

    std::cout << other.getName() << ", do you accept the trade? (yes/no): ";
    std::string response;
    std::cin >> response;

    if (response == "yes") {
        for (size_t i = 0; i < offer.size(); ++i) {
            resources[offer[i]] -= offerAmounts[i];
            other.resources[offer[i]] += offerAmounts[i];
        }
        for (size_t i = 0; i < request.size(); ++i) {
            other.resources[request[i]] -= requestAmounts[i];
            resources[request[i]] += requestAmounts[i];
        }
        std::cout << "Trade completed successfully." << std::endl;
    } else {
        std::cout << "Trade declined." << std::endl;
    }
}

void Player::buyDevelopmentCard() {
    static const std::map<std::string, int> cardCost = {{"ore", 1}, {"wool", 1}, {"wheat", 1}};

    if (!hasResources(cardCost)) {
        throw std::runtime_error("Not enough resources to buy a development card");
    }

    deductResources(cardCost);

    // Generate a random development card type (0-4)
    int cardType = rand() % 5;
    developmentCards[cardType]++;
    switch (cardIndex) {
        case 0:
            std::cout << name << " bought a development card of type monopoly" << std::endl;
            break;
        case 1:
            std::cout << name << " bought a development card of type road building" << std::endl;
            break;
        case 2:
            std::cout << name << " bought a development card of type year of plenty" << std::endl;
            break;
        case 3:
            std::cout << name << " bought a development card of type knight" << std::endl;
            break;
        case 4:
            std::cout << name << " bought a development card of type victory point" << std::endl;
            points++;
            break;
        default:
            throw std::out_of_range("Invalid development card index");
    }
}



void Player::printPoints() const {
    std::cout << name << " has " << points << " points." << std::endl;
}

void Player::useMonopoly(Catan &game) {
    std::string resourceName;
    std::cout << "Enter a resource name (wood, brick, wheat, ore, wool): ";
    std::cin >> resourceName;

    // Validate resource name
    while (resources.find(resourceName) == resources.end()) {
        std::cout << "Invalid resource name. Enter a valid resource name: ";
        std::cin >> resourceName;
    }

    int totalTaken = 0;
    // Collect all instances of this resource from other players
    for (int playerId = 1; playerId <= 3; ++playerId) {
        if (playerId == color) continue; // Skip current player
        Player &currentPlayer = game.getPlayer(playerId);

        // Take the resource amount from other players
        int taken = currentPlayer.resources[resourceName];
        resources[resourceName] += taken;
        currentPlayer.resources[resourceName] = 0;
        totalTaken += taken;
    }

    std::cout << name << " used a monopoly card and gained " << totalTaken << " " << resourceName << " from other players." << std::endl;
    game.nextTurn();
}

void Player::useRoadBuilding(Catan &game) {
    if(developmentCards[1] == 0) {
        std::cout << "You don't have any road building cards to use." << std::endl;
        return;
    }

    int validAttempts = 0;

    int landIndex; 
    int roadIndex;

    while (validAttempts < 2) {
        std::cout << "Enter land index and road index to place road (e.g., 0 1): ";
        std::cin >> landIndex >> roadIndex;

        if (landIndex >= 0 && landIndex < 19 && roadIndex >= 0 && roadIndex < 6) {
            Land &land = game.getBoard().getLand(landIndex);
            if (land.occupyRoad(roadIndex, color)) {
                lands[landIndex] = true;
                std::cout << name << " has placed a road on land " << landIndex << " at road " << roadIndex << "." << std::endl;
                validAttempts++;
            } else {
                std::cout << "Invalid indexes. Try again." << std::endl;
            }
        } else {
            std::cout << "Invalid indexes. Try again." << std::endl;
        }
    }

    developmentCards[1]--; // Decrease Road Building card count
    game.nextTurn();
}


void Player::useYearOfPlenty(Catan &game) {
    if(developmentCards[2] == 0) {
        std::cout << "You don't have any year of plenty cards to use." << std::endl;
        return;
    }

    std::string resourceName;
    int validAttempts = 0;

    while (validAttempts < 2) {
        std::cout << "Enter a resource name to receive (wood, brick, wheat, ore, wool): ";
        std::cin >> resourceName;

        if (resources.find(resourceName) != resources.end()) {
            resources[resourceName]++;
            std::cout << "Received 1 " << resourceName << "." << std::endl;
            validAttempts++;
        } else {
            std::cout << "Invalid resource name. Try again." << std::endl;
        }
    }

    developmentCards[2]--; // Decrease Year of Plenty card count
    game.nextTurn();
}



void Player::useKnight(Catan &game) {
    if (developmentCards[3] == 0) {
        std::cout << "You don't have any knight cards to use." << std::endl;
        return;
    }

    developmentCards[3]--;
    usedKnights++;

    // Move robber to a location chosen by command line input
    int robberPlacement;
    std::cout << "Choose a number between 0-18 to place the robber: ";
    std::cin >> robberPlacement;

    // Validate robber placement
    while (robberPlacement < 0 || robberPlacement > 18) {
        std::cout << "Invalid request. Please choose a number between 0-18: ";
        std::cin >> robberPlacement;
    }

    // Move the robber
    game.getBoard().placeRobber(robberPlacement);

    std::cout << name << " used a knight card and moved the robber to location " << robberPlacement << std::endl;

    // Check if current player has more knights than the current mostKnights
    if (usedKnights > game.mostKnights && usedKnights > 2) {
        // Decrease former player's points by 2 if there was a previous leader
        if (game.mostKnights > 0) {
            game.getPlayer(game.mostKnightsPlayer).points -= 2;
        }
        // Update game's mostKnights and mostKnightsPlayer
        game.mostKnights = usedKnights;
        game.mostKnightsPlayer = color;
        // Increase current player's points by 2
        points += 2;
    }

    game.nextTurn();
}