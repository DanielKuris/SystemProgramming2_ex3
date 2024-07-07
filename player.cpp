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
    : name(name), color(color), points(0), lands(19, false), developmentCards(5, 0) {
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

void Player::placeSettlement(int landIndex, int spotIndex, Catan &game) {
    static const std::map<std::string, int> settlementCost = {{"brick", 1}, {"wood", 1}, {"wool", 1}, {"wheat", 1}};
    
    if (!hasResources(settlementCost)) {
        throw std::runtime_error("Not enough resources to place a settlement");
    }

    Land &land = game.getBoard().getLand(landIndex);
    if (!land.isSpotAvailable(spotIndex) || !land.isConnected(spotIndex, color)) {
        throw std::runtime_error("Spot not available for settlement or not connected to a road");
    }

    land.occupySpot(spotIndex, color);
    lands[landIndex] = true;
    points++;
    deductResources(settlementCost);
    std::cout << name << " placed a settlement on land " << landIndex << " at spot " << spotIndex << std::endl;
}

void Player::placeRoad(int landIndex, int pathIndex, Catan &game) {
    static const std::map<std::string, int> roadCost = {{"brick", 1}, {"wood", 1}};

    if (!hasResources(roadCost)) {
        throw std::runtime_error("Not enough resources to place a road");
    }

    Land &land = game.getBoard().getLand(landIndex);
    if (!land.isPathAvailable(pathIndex) || !land.isConnected(pathIndex, color)) {
        throw std::runtime_error("Path not available for road or not connected to a settlement/road/city");
    }

    land.occupyPath(pathIndex, color);
    deductResources(roadCost);
    std::cout << name << " placed a road on land " << landIndex << " at path " << pathIndex << std::endl;
}

void Player::upgradeSettlement(int landIndex, int spotIndex, Catan &game) {
    static const std::map<std::string, int> cityCost = {{"ore", 3}, {"wheat", 2}};

    if (!hasResources(cityCost)) {
        throw std::runtime_error("Not enough resources to upgrade to a city");
    }

    Land &land = game.getBoard().getLand(landIndex);
    if (!lands[landIndex] || land.getSpotOwner(spotIndex) != color) {
        throw std::runtime_error("No settlement to upgrade or not owned by the player");
    }

    land.upgradeSpotToCity(spotIndex, color);
    points++;
    deductResources(cityCost);
    std::cout << name << " upgraded a settlement to a city on land " << landIndex << " at spot " << spotIndex << std::endl;
}

void Player::rollDice(Catan &game) {
    int diceRoll = rand() % 11 + 2; // Generates a random number from 2 to 12

    if (diceRoll == 7) {
        std::cout << "Dice rolled: " << diceRoll << std::endl;
        int robberPlacement;
        std::cout << "Choose a number between 1-19 to place the robber: ";
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
            if (land.getNumber() == diceRoll && !game.getBoard().hasRobber(landIndex)) {
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
        std::cout << name << "'s resources collected:" << std::endl;
        for (const auto &res : resources) {
            std::cout << res.second << " " << res.first << std::endl;
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

    // Generate a random development card type (0-3, excluding victoryPoints)
    int cardType = rand() % 4;
    developmentCards[cardType]++;
    std::cout << name << " bought a development card of type " << cardType << std::endl;
}

void Player::useDevelopmentCard(int cardIndex, Catan &game) {
    if (cardIndex < 0 || cardIndex > 4) {
        throw std::out_of_range("Invalid development card index");
    }

    switch (cardIndex) {
        case 0:
            useMonopoly();
            break;
        case 1:
            useRoadBuilding(game);
            break;
        case 2:
            useYearOfPlenty();
            break;
        case 3:
            useKnight();
            break;
        default:
            throw std::out_of_range("Invalid development card index");
    }
}

void Player::printPoints() const {
    std::cout << name << " has " << points << " points." << std::endl;
}

void Player::useMonopoly() {
    developmentCards[0]--;
    // Implement monopoly card effect here
    std::cout << name << " used a monopoly card." << std::endl;
}

void Player::useRoadBuilding(Catan &game) {
    developmentCards[1]--;
    // Implement road building card effect here
    std::cout << name << " used a road building card." << std::endl;
}

void Player::useYearOfPlenty() {
    developmentCards[2]--;
    // Implement year of plenty card effect here
    std::cout << name << " used a year of plenty card." << std::endl;
}

void Player::useKnight() {
    developmentCards[3]--;
    // Implement knight card effect here
    std::cout << name << " used a knight card." << std::endl;
}
