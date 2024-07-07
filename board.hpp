// board.hpp
#ifndef BOARD_HPP
#define BOARD_HPP

#include "land.hpp"
#include <vector>

class Board {
private:
    std::vector<Land> lands; // Vector of 19 lands
    int robberLocation; // Current location of the robber
    std::vector<std::vector<int>> neighbors; // List of neighbors for each land

public:
    Board();
    Land& getLand(int index);
    void placeRobber(int landIndex);
    int getRobberLocation() const;
};

#endif
