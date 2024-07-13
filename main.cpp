// danielkuris6@gmail.com

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm> // for std::random_shuffle
#include "catan.hpp"

// Function to handle player actions until "pass" is given
void handlePlayerActions(Player& currentPlayer, Catan& catanGame, Board& catanBoard) {
    std::string action;

    while (true) {
        std::cout << "What would you like to do? (pass, build, buy, trade, use, help): ";
        std::cin >> action;

        if (action == "pass") {
            break; // Exit the loop on "pass"
        } else if (action == "build") {
            std::string buildType;
            std::cout << "What would you like to build? (city, settlement, road): ";
            std::cin >> buildType;

            if (buildType == "city") {
                int landIndex, settlementIndex;
                std::cout << "Enter land index and settlement index: ";
                std::cin >> landIndex >> settlementIndex;
                currentPlayer.upgradeSettlement(landIndex, settlementIndex);
            } else if (buildType == "settlement") {
                int landIndex, spotIndex;
                std::cout << "Enter land index and spot index: ";
                std::cin >> landIndex >> spotIndex;
                currentPlayer.placeSettlement(landIndex, spotIndex);
            } else if (buildType == "road") {
                int landIndex, roadIndex;
                std::cout << "Enter land index and road index: ";
                std::cin >> landIndex >> roadIndex;
                currentPlayer.placeRoad(landIndex, roadIndex);
            } else {
                std::cout << "Invalid build type. Try again." << std::endl;
            }
        } else if (action == "buy") {
            currentPlayer.buyDevelopmentCards();
        } else if (action == "trade") {
            std::string playerName, offer, request;
            std::cout << "Enter player name, offer (e.g., '4 wool'), and request: ";
            std::cin >> playerName >> offer >> request;
            currentPlayer.trade(playerName, offer, request);
        } else if (action == "use") {
            std::string cardName;
            std::cout << "Which development card would you like to use? ";
            std::cin >> cardName;

            if (cardName == "knight") {
                currentPlayer.useKnight(catanBoard);
            } else if (cardName == "year_of_plenty") {
                std::string resource1, resource2;
                std::cout << "Enter two resources to take (e.g., 'wool grain'): ";
                std::cin >> resource1 >> resource2;
                currentPlayer.useYearOfPlenty(resource1, resource2);
            } else if (cardName == "road_building") {
                int landIndex1, roadIndex1, landIndex2, roadIndex2;
                std::cout << "Enter two pairs of land index and road index: ";
                std::cin >> landIndex1 >> roadIndex1 >> landIndex2 >> roadIndex2;
                currentPlayer.useRoadBuilding(landIndex1, roadIndex1, landIndex2, roadIndex2);
            } else if (cardName == "monopoly") {
                std::string resourceType;
                std::cout << "Enter resource type to monopolize (e.g., 'ore'): ";
                std::cin >> resourceType;
                currentPlayer.useMonopoly(resourceType);
            } else {
                std::cout << "Invalid development card. Try again." << std::endl;
            }
        } else if (action == "help") {
            // Print available commands
            std::cout << "Available commands: pass, build, buy, trade, use, help" << std::endl;
        } else {
            std::cout << "Invalid action. Try again." << std::endl;
        }
    }
}

int main() {
    // Initialize game names and board
    std::string player1 = "Player 1";
    std::string player2 = "Player 2";
    std::string player3 = "Player 3";
    Catan catanGame(player1, player2, player3);

    // Set up the board with 19 lands
    Board catanBoard;

    // Set up probabilities and types for each land
    std::vector<std::string> landTypes = {"wool", "grain", "brick", "ore", "wood"};
    std::vector<int> diceProbabilities = {3, 4, 5, 6, 8, 9, 10, 11, 3, 4, 5, 6, 8, 9, 10, 11, 2, 12};

    // Randomize the placement of types and probabilities
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Seed for randomization
    std::random_shuffle(landTypes.begin(), landTypes.end());
    std::random_shuffle(diceProbabilities.begin(), diceProbabilities.end());

    // Randomly choose a desert index
    int desertIndex = std::rand() % 19;

    // Add lands to the board
    for (int i = 0; i < 19; ++i) {
        std::string type;
        int probability;

        if (i == desertIndex) {
            type = "desert";
            probability = 0;
        } else {
            type = landTypes.back();
            landTypes.pop_back();
            probability = diceProbabilities.back();
            diceProbabilities.pop_back();
        }

        catanBoard.getLand(i) = Land(type, i, probability);
    }

    // Starting player
    catanGame.ChooseStartingPlayer();

    // Initial occupy for each player
    for (int i = 0; i < 3; ++i) {
        Player& currentPlayer = catanGame.getCurrentPlayer();
        currentPlayer.initialOccupy(catanBoard);
        catanGame.nextTurn();  // Move to the next player's turn
    }

    // Game loop until a winner is determined
    for(int i = 0; i < 3; ++i) {
        Player& currentPlayer = catanGame.getCurrentPlayer();

        // Player rolls the dice
        currentPlayer.rollDice();

        // Handle player actions until they pass
        handlePlayerActions(currentPlayer, catanGame, catanBoard);

        // Move to the next player's turn
        catanGame.nextTurn();
    }

    return 0;
}
