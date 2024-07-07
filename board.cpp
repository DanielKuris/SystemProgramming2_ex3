#include "board.hpp"
#include <stdexcept>

Board::Board() : lands{}, robberLocation(-1), neighbors {
    {1, 4, 3, -1, -1, -1},   // Land 0
    {2, 5, 4, 0, -1, -1},    // Land 1
    {-1, 6, 5, 1, -1, -1},   // Land 2
    {4, 8, 7, -1, -1, 0},    // Land 3
    {5, 9, 8, 3, 0, 1},      // Land 4
    {6, 10, 9, 4, 1, 2},     // Land 5
    {-1, 11, 10, 5, 2, -1},  // Land 6
    {8, 12, -1, -1, -1, 3},  // Land 7
    {9, 13, 12, 7, 3, 4},    // Land 8
    {10, 14, 13, 8, 4, 5},   // Land 9
    {11, 15, 14, 9, 5, 6},   // Land 10
    {-1, -1, 15, 10, 6, -1}, // Land 11
    {13, 16, -1, -1, 7, 8},  // Land 12
    {14, 17, 16, 12, 8, 9},  // Land 13
    {15, 18, 17, 13, 9, 10}, // Land 14
    {-1, -1, 18, 14, 10, 11},// Land 15
    {17, -1, -1, -1, 12, 13},// Land 16
    {18, -1, -1, 16, 13, 14},// Land 17
    {-1, -1, -1, 17, 14, 15} // Land 18
} {}

Land& Board::getLand(int index) {
    if (index < 0 || index >= 19) {
        throw std::out_of_range("Invalid land index");
    }
    return lands[index];
}

void Board::placeRobber(int landIndex) {
    if (landIndex < 0 || landIndex >= 19) {
        throw std::out_of_range("Invalid land index");
    }
    robberLocation = landIndex;
}

int Board::getRobberLocation() const {
    return robberLocation;
}

