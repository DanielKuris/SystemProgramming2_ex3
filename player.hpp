// danielkuris6@gmail.com

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <map>

// Forward declarations
class Catan;
class Board;

class Player {
private:
    std::string name;  // Player's name
    int color; // Unique color identifier for the player
    std::map<std::string, int> resources;  // Resource map {resource name, amount}
    int points;  // Player's victory points
    std::vector<bool> lands; // size 19, true if player has a settlement on the land
    std::vector<int> developmentCards; // {monopoly, roadBuilding, yearOfPlenty, knights, victoryPoints}
    int usedKnights;  // Number of knights used

    bool hasResources(const std::map<std::string, int> &cost) const;  // Check if player has enough resources to perform an action
    void deductResources(const std::map<std::string, int> &cost);  // Deduct resources from player's inventory
    void useMonopoly(Catan &game);  // Use monopoly card to take all instances of a specific resource from other players
    void useRoadBuilding(Catan &game);  // Use road building card to place two roads
    void useYearOfPlenty(Catan &game);  // Use year of plenty card to take two resources of choice from the bank
    void useKnight(Catan &game);  // Use knight card to move the robber and increase usedKnights

public:
    Player(const std::string &name, int color);  // Constructor to initialize player with name and color
    const std::string& getName() const;  // Get player's name
    int getColor() const;  // Get player's color
    void placeSettlement(int landIndex, int spotIndex, Catan &game);  // Place a settlement on a specific land and spot
    void placeRoad(int landIndex, int roadIndex, Catan &game);  // Place a road on a specific land and road
    void upgradeSettlement(int landIndex, int spotIndex, Catan &game);  // Upgrade a settlement to a city on a specific land and spot
    void rollDice(Catan &game);  // Roll the dice and collect resources based on dice result
    void endTurn(Catan &game);  // End the player's turn
    void trade(Player &other, const std::vector<std::string> &offer, const std::vector<int> &offerAmounts, const std::vector<std::string> &request, const std::vector<int> &requestAmounts);  // Trade resources with another player
    void buyDevelopmentCard();  // Buy a development card from the bank
    void printPoints() const;  // Print player's current victory points
};

#endif
