// player.hpp
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <map>

class Catan;
class Board;

class Player {
private:
    std::string name;
    int color; // Unique color identifier for the player
    std::map<std::string, int> resources;
    int points;
    std::vector<bool> lands; // size 19, true if player has a settlement on the land
    std::vector<int> developmentCards; // {monopoly, roadBuilding, yearOfPlenty, knights, victoryPoints}

    bool hasResources(const std::map<std::string, int> &cost) const;
    void deductResources(const std::map<std::string, int> &cost);
    void useMonopoly();
    void useRoadBuilding(Catan &game);
    void useYearOfPlenty();
    void useKnight();

public:
    Player(const std::string &name, int color);
    const std::string& getName() const;
    int getColor() const;
    void placeSettlement(int landIndex, int spotIndex, Catan &game);
    void placeRoad(int landIndex, int pathIndex, Catan &game);
    void upgradeSettlement(int landIndex, int spotIndex, Catan &game);
    void rollDice(Catan &game, Board &board);
    void endTurn(Catan &game);
    void trade(Player &other, const std::vector<std::string> &offer, const std::vector<int> &offerAmounts, const std::vector<std::string> &request, const std::vector<int> &requestAmounts);
    void buyDevelopmentCard();
    void useDevelopmentCard(int cardIndex, Catan &game);
    void printPoints() const;
};

#endif
