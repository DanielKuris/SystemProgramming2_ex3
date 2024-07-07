// main.cpp
#include <iostream>
#include "player.hpp"
#include "board.hpp"
#include "catan.hpp"

int main() {
    Catan catan("Amit", "Yossi", "Dana");

    catan.ChooseStartingPlayer();

    Player &currentPlayer = catan.getCurrentPlayer();
    
    // Example of placing settlements and roads through the Catan game instance
    try {
        currentPlayer.placeSettlement(0, 0, catan);
        currentPlayer.placeRoad(0, 0, catan);
        currentPlayer.endTurn(catan);

        currentPlayer = catan.getCurrentPlayer();
        currentPlayer.placeSettlement(1, 1, catan);
        currentPlayer.placeRoad(1, 1, catan);
        currentPlayer.endTurn(catan);

        currentPlayer = catan.getCurrentPlayer();
        currentPlayer.buyDevelopmentCard();
        currentPlayer.useDevelopmentCard(0, catan); // Example of using a development card
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
