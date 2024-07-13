// danielkuris6@gmail.com

// catan.cpp
#include "catan.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Catan::Catan(const std::string &name1, const std::string &name2, const std::string &name3)
    : currentPlayerIndex(0), mostKnights(0), mostKnightsPlayer(0) {
    players.push_back(Player(name1, 1));
    players.push_back(Player(name2, 2));
    players.push_back(Player(name3, 3));
    srand(time(0)); // Seed for random starting player
}

void Catan::ChooseStartingPlayer() {
    currentPlayerIndex = rand() % players.size();
    std::cout << "Starting player: " << players[currentPlayerIndex].getName() << std::endl;
}

Board& Catan::getBoard() {
    return board;
}

void Catan::printWinner() const {
    for (const auto &player : players) {
        if (player.getPoints() >= 10) {
            std::cout << "Player " << player.getName() << " is the winner!" << std::endl;
            return;
        }
    }
}

Player& Catan::getCurrentPlayer() {
    return players[currentPlayerIndex];
}

void Catan::nextTurn() {
    currentPlayerIndex = (currentPlayerIndex % players.size()) + 1;
    std::cout << "Next player's turn: " << players[currentPlayerIndex - 1].getName() << std::endl;

    // Check if current player is the winner
    if (players[currentPlayerIndex - 1].getPoints() >= 10) {
        std::cout << "Player " << players[currentPlayerIndex - 1].getName() << " is the winner!" << std::endl;
    }
}

Player& Catan::getPlayer(int playerId) {
    for (auto &player : players) {
        if (player.getId() == playerId) {
            return player;
        }
    }
    throw std::out_of_range("Invalid player ID");
}
