// danielkuris6@gmail.com

#ifndef CATAN_HPP
#define CATAN_HPP

#include "board.hpp"
#include "player.hpp"

#include <vector>
#include <string>

class Catan {
private:
    Board board;
    std::vector<Player> players;
    int currentPlayerIndex;
    int mostKnights;
    int mostKnightsPlayer;

public:
    Catan(const std::string &name1, const std::string &name2, const std::string &name3);
    void ChooseStartingPlayer();
    Board& getBoard();
    void printWinner() const;
    Player& getCurrentPlayer();
    void nextTurn();
    Player& getPlayer(int playerId);
};

#endif // CATAN_HPP

