#ifndef BOARD_HPP
#define BOARD_HPP

#include "land.hpp" // Include the necessary header for Land class

#include <array>    // Include for std::array
#include <stdexcept> // Include for std::out_of_range exception

class Board {
private:
    std::array<Land, 19> lands; // Array of 19 Land objects
    int robberLocation;          // Index of the land where the robber is located
    const std::array<std::array<int, 6>, 19> neighbors; // Array representing neighbors for each land

public:
    Board(); // Constructor

    Land& getLand(int index); // Getter for accessing a specific Land object by index
    void placeRobber(int landIndex); // Sets the robber's location to a specified land index
    int getRobberLocation() const; // Getter for obtaining the current location of the robber
};

#endif // BOARD_HPP
