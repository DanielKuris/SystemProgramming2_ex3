#ifndef LAND_HPP
#define LAND_HPP

#include <vector>
#include <string>
#include <iostream>
#include "catan.hpp"

class Land {
private:
    std::string type;
    int number;
    std::vector<int> settlements; // size 6, 0 if no settlement, player's color if occupied
    std::vector<int> cities;      // size 6, 0 if no city, player's color if occupied
    std::vector<int> roads;       // size 6, 0 if no road, player's color if occupied

public:
    Land(const std::string &type, int number);
    const std::string& getType() const;
    int getNumber() const;
    void occupySpot(int spotIndex, int playerColor, Catan &game);
    void occupyNeighborsSpot(int spotIndex, int playerColor, Catan &game);
    void occupyRoad(int roadIndex, int playerColor, Catan &game);
    void occupyNeighborsRoad(int roadIndex, int playerColor, Catan &game);
    void upgradeToCity(int spotIndex, int playerColor, Catan &game);
    void occupyNeighborsCity(int spotIndex, int playerColor, Catan &game);
    bool isValidCity(int spotIndex, int playerColor, Catan &game) const;
    bool isValidRoad(int roadIndex, int playerColor, Catan &game) const;
    bool isValidSettlement(int setIndex, int playerColor, Catan &game) const;
};

#endif // LAND_HPP
