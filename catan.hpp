#ifndef CATAN_HPP
#define CATAN_HPP

#include "board.hpp" // Include the necessary header for Board class
#include "player.hpp" // Include the necessary header for Player class

#include <vector>
#include <string>

class Catan {
private:
    Board board;                     // Game board
    std::vector<Player> players;     // Vector of players
    int currentPlayerIndex;          // Index of the current player
    int mostKnights;                 // Most knights owned by any player
    int mostKnightsPlayer;           // Player with the most knights

public:
    Catan(const std::string &name1, const std::string &name2, const std::string &name3); // Constructor
    void ChooseStartingPlayer();     // Chooses a random starting player
    Board& getBoard();               // Getter for the game board
    void printWinner() const;        // Prints the winner if any player reaches 10 points
    Player& getCurrentPlayer();      // Getter for the current player
    void nextTurn();                 // Moves to the next player's turn
    Player& getPlayer(int playerId); // Getter for a specific player by ID
};

#endif // CATAN_HPP
