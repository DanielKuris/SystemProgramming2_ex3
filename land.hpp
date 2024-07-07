#ifndef LAND_HPP
#define LAND_HPP

#include <vector>
#include <string>
#include "catan.hpp"

class Land {
private:
    std::string type;
    int number; // Index from 0 to 18
    int probability; // probability from 2 to 12
    std::vector<int> settlements; // size 6, 0 if no settlement, player's color if occupied
    std::vector<int> cities;      // size 6, 0 if no city, player's color if occupied
    std::vector<int> roads;       // size 6, 0 if no road, player's color if occupied

public:
    Land(const std::string &type, int number, int probability);
    const std::string& getType() const;
    int getNumber() const;
    int getProbability() const;
    bool occupySpot(int spotIndex, int playerColor, Catan &game);
    void occupyNeighborsSpot(int spotIndex, int playerColor, Catan &game);
    bool occupyRoad(int roadIndex, int playerColor, Catan &game);
    void occupyNeighborsRoad(int roadIndex, int playerColor, Catan &game);
    bool upgradeToCity(int spotIndex, int playerColor, Catan &game);
    void occupyNeighborsCity(int spotIndex, int playerColor, Catan &game);
    bool isValidCity(int spotIndex, int playerColor, Catan &game) const;
    bool isValidRoad(int roadIndex, int playerColor, Catan &game) const;
    bool isValidSettlement(int setIndex, int playerColor, Catan &game) const;
};

#endif // LAND_HPP
