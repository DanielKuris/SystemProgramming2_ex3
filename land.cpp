#include "land.hpp"


Land::Land(const std::string &type, int number, int probability)
    : type(type), number(number), probability(probability), settlements(6, 0), cities(6, 0), roads(6, 0) {}

const std::string& Land::getType() const {
    return type;
}

int Land::getNumber() const {
    return number;
}

int Land::getProbability() const {
    return probability;
}

bool Land::occupySpot(int spotIndex, int playerColor, Catan &game) {
    if(spotIndex < 0 || spotIndex >= 6) {
        std::cout << "Invalid spot index, cancelled settlement building" << std::endl;
        return false;
    }

    if(isValidSettlement(spotIndex, playerColor, game)) {
        settlements[spotIndex] = playerColor;
        occupyNeighborsSpot(spotIndex, playerColor, game);
        return true;
    }
    else {
        std::cout << "Invalid settlement placement, cancelled settlement building" << std::endl;
        return false;
    }
}

void Land::occupyNeighborsSpot(int spotIndex, int playerColor, Catan &game) {
    auto& neighbors = game.getBoard().neighbors;

    int neighborIndex = (spotIndex-2+6)%6;
    int landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedSpot = (spotIndex+2)%6;
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = playerColor;
    }

    neighborIndex = (neighborIndex + 1) % 6;
    landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedSpot = (spotIndex-2+6)%6;
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = playerColor;
    }
}

bool Land::occupyRoad(int roadIndex, int playerColor, Catan &game) {
    if(roadIndex < 0 || roadIndex >= 6) {
        std::cout << "Invalid spot index, cancelled settlement building" << std::endl;
        return false;
    }

    if(isValidRoad(roadIndex, playerColor, game)) {
        roads[roadIndex] = playerColor;
        occupyNeighborsRoad(roadIndex, playerColor, game);
        return true;
    }
    else {
        std::cout << "Invalid settlement placement, cancelled settlement building" << std::endl;
        return false;
    }
}

void Land::occupyNeighborsRoad(int roadIndex, int playerColor, Catan &game) {
    auto& neighbors = game.getBoard().neighbors;

    int neighborIndex = (roadIndex-1+6)%6;
    int landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedEdgeIndex = (neighborIndex+4)%6;
        game.getBoard().getLand(neighborIndex).roads[sharedEdgeIndex] = playerColor;
    }
}

bool Land::upgradeToCity(int spotIndex, int playerColor, Catan &game) {
    if(spotIndex < 0 || spotIndex >= 6) {
        std::cout << "Invalid spot index, cancelled city building" << std::endl;
        return false;
    }

    if(isValidCity(spotIndex, playerColor, game)) {
        cities[spotIndex] = playerColor;
        settlements[spotIndex] = 0;
        occupyNeighborsCity(spotIndex, playerColor, game);
        return true;
    }
    else {
        std::cout << "Invalid settlement placement, cancelled city building" << std::endl;
        return false;
    }
}

void Land::occupyNeighborsCity(int spotIndex, int playerColor, Catan &game) {
    auto& neighbors = game.getBoard().neighbors;

    int neighborIndex = (spotIndex-2+6)%6;
    int landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedSpot = (spotIndex+2)%6;
        game.getBoard().getLand(neighborIndex).cities[sharedSpot] = playerColor;
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = 0;
    }

    neighborIndex = (neighborIndex + 1) % 6;
    landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedSpot = (spotIndex-2+6)%6;
        game.getBoard().getLand(neighborIndex).cities[sharedSpot] = playerColor;
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = 0;
    }
}

bool Land::isValidCity(int spotIndex, int playerColor, Catan &game) const {
    if (settlements[spotIndex] == playerColor && cities[spotIndex] == 0) {
        return true;
    }
    return false;
}

bool Land::isValidRoad(int roadIndex, int playerColor, Catan &game) const {
    if (roads[roadIndex] != 0) {
        std::cout << "Invalid request: Road index " << roadIndex << " is already occupied. Cancelling road building." << std::endl;
        return false;
    }

    if (settlements[roadIndex] == playerColor || cities[roadIndex] == playerColor ||
        settlements[(roadIndex+1)%6] == playerColor || cities[(roadIndex+1)%6] == playerColor) {
         return true;
    }

    auto& neighbors = game.getBoard().neighbors;

    int neighborIndex = (roadIndex-2+6)%6;
    int landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedEdgeIndex = (neighborIndex+4)%6;
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex-1+6)%6] == playerColor) {
            return true;
        }
    }

    neighborIndex = (neighborIndex + 1) % 6;
    landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedEdgeIndex = (neighborIndex+4)%6;
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex-1+6)%6] == playerColor ||
            game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex+1+6)%6] == playerColor) {
            return true;
        }
    }

    neighborIndex = (neighborIndex + 1) % 6;
    landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedEdgeIndex = (neighborIndex+4)%6;
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex+1+6)%6] == playerColor) {
            return true;
        }
    }

    return false;
}

bool Land::isValidSettlement(int setIndex, int playerColor, Catan &game) const {
    if (settlements[setIndex] != 0 || cities[setIndex] != 0) {
        std::cout << "Invalid request: Settlement index " << setIndex << " is already occupied. Cancelling settlement building." << std::endl;
        return false;
    }

    if (settlements[(setIndex-1)%6] != 0 || cities[(setIndex-1)%6] != 0 ||
        settlements[(setIndex+1)%6] != 0 || cities[(setIndex+1)%6] != 0) {
         return false;
    }

    bool hasNearbyRoad = false;

    if (roads[(setIndex-1)%6] == playerColor || roads[(setIndex)%6] == playerColor) {
         hasNearbyRoad = true;
    }

    auto& neighbors = game.getBoard().neighbors;

    int neighborIndex = (setIndex-2+6)%6;
    int landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedEdgeIndex = (neighborIndex+4)%6;
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex-1+6)%6] == playerColor) {
            hasNearbyRoad = true;
        }

        if (game.getBoard().getLand(neighborIndex).settlements[(sharedEdgeIndex-1+6)%6] != 0 ||
            game.getBoard().getLand(neighborIndex).cities[(sharedEdgeIndex-1+6)%6] != 0) {
            return false;
        }
    }

    neighborIndex = (neighborIndex + 1) % 6;
    landIndex = neighbors[number][neighborIndex];

    if (landIndex != -1) {
        int sharedEdgeIndex = (neighborIndex+4)%6;
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex+1+6)%6] == playerColor) {
            hasNearbyRoad = true;
        }

        if (game.getBoard().getLand(neighborIndex).settlements[(sharedEdgeIndex+2+6)%6] != 0 ||
            game.getBoard().getLand(neighborIndex).cities[(sharedEdgeIndex+2+6)%6] != 0) {
            return false;
        }
    }

    return true && hasNearbyRoad;
}
