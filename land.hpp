#ifndef LAND_HPP
#define LAND_HPP

#include <vector>
#include <string>
#include "catan.hpp" // Include the necessary header for Catan class
#include <utility>   // For std::pair
#include <set>       // For std::set

class Land {
private:
    std::string type;                 // Type of land
    int number;                       // Index from 0 to 18
    int probability;                  // Probability from 2 to 12
    std::vector<int> settlements;     // Size 6, 0 if no settlement, player's color if occupied
    std::vector<int> cities;          // Size 6, 0 if no city, player's color if occupied
    std::vector<int> roads;           // Size 6, 0 if no road, player's color if occupied

public:
    Land(const std::string &type, int number, int probability);  // Constructor
    const std::string& getType() const;                         // Getter for land type
    int getNumber() const;                                      // Getter for land number
    int getProbability() const;                                 // Getter for land probability
    bool occupySpot(int spotIndex, int playerColor, Catan &game); // Occupies a settlement spot
    void occupyNeighborsSpot(int spotIndex, int playerColor, Catan &game); // Occupies neighbors of settlement
    bool occupyRoad(int roadIndex, int playerColor, Catan &game); // Occupies a road spot
    void occupyNeighborsRoad(int roadIndex, int playerColor, Catan &game); // Occupies neighbors of road
    bool upgradeToCity(int spotIndex, int playerColor, Catan &game); // Upgrades settlement to city
    void occupyNeighborsCity(int spotIndex, int playerColor, Catan &game); // Occupies neighbors of city
    bool isValidCity(int spotIndex, int playerColor, Catan &game) const; // Checks if city placement is valid
    bool isValidRoad(int roadIndex, int playerColor, Catan &game) const; // Checks if road placement is valid
    bool isValidSettlement(int setIndex, int playerColor, Catan &game) const; // Checks if settlement placement is valid
    std::vector<std::pair<int, int>> isValidSettlementInitial(int setIndex, int playerColor, Catan &game) const; // Checks initial settlement placement

};

#endif // LAND_HPP
