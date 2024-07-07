#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include "land.hpp"  // Include necessary headers

class Board {
private:
    std::array<Land, 19> lands;
    int robberLocation;
    std::array<std::array<int, 6>, 19> neighbors;

public:
    Board();  // Constructor declaration
    
    Land& getLand(int index);
    void placeRobber(int landIndex);
    int getRobberLocation() const;
};

#endif // BOARD_HPP

