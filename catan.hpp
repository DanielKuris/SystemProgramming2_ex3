#ifndef CATAN_HPP
#define CATAN_HPP

#include "player.hpp"
#include "board.hpp"
#include <vector>
#include <string>

class Catan {
private:
    std::vector<Player> players;
    Board board;

public:
    Catan(const std::string &name1, const std::string &name2, const std::string &name3);

    // Function declarations
    Player& getPlayer(int playerId);
    Board& getBoard();

    // Other functions as needed
    void ChooseStartingPlayer();
    void printWinner() const;
    Player& getCurrentPlayer();
    void nextTurn();
};

#endif // CATAN_HPP
