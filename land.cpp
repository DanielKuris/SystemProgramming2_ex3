#include "land.hpp"
#include "catan.hpp"

Land::Land(const std::string &type, int number) : type(type), number(number), settlements(6, 0), cities(6, 0), roads(6, 0) {}

const std::string& Land::getType() const {
    return type;
}

int Land::getNumber() const {
    return number;
}

void Land::occupySpot(int spotIndex, int playerColor, Catan &game) {
    if(spotIndex < 0 || spotIndex >= 6) {
        std :: cout << "Invalid spot index, cancelled settlement building" << std :: endl;
        return;
    }

    if(isValidSettlement(spotIndex, playerColor, game)) {
        settlements[spotIndex] = playerColor;
        occupyNeighborsSpot(spotIndex, playerColor, game);
    }
    else {
        std :: cout << "Invalid settlement placement, cancelled settlement building" << std :: endl;
    }
}

void Land::occupyNeighborsSpot(int spotIndex, int playerColor, Catan &game) {
    // Get current land's neighbors
    auto& neighbors = game.getBoard().neighbors;
    
    // Start update settlements for first neighbor:

    // First neighbor's index is the neighbor that shares edge at one counter-clockwise from requested road
    int neighborIndex = (spotIndex-2+6)%6;

    // Get first neighbor's land index
    int landIndex = neighbors[number][neighborIndex];
    
    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared spot index at neighbor's land
        int sharedSpot = (spotIndex+2)%6;

        // Update neighbor's shared spot
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = playerColor;
    }

    // Start update settlements for second neighbor:

    // Second neighbor's index is the neighbor that shares edge from requested road
    int neighborIndex = (neighborIndex + 1) % 6;

    // Get second neighbor's land index
    int landIndex = neighbors[number][neighborIndex];

    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared spot index at neighbor's land
        int sharedSpot = (spotIndex-2+6)%6;

        // Update neighbor's shared spot
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = playerColor;
    }
}

void Land::occupyRoad(int roadIndex, int playerColor, Catan &game) {
    if(roadIndex < 0 || roadIndex >= 6) {
        std :: cout << "Invalid spot index, cancelled settlement building" << std :: endl;
        return;
    }

    if(isValidRoad(roadIndex, playerColor, game)) {
        roads[roadIndex] = playerColor;
        occupyNeighborsRoad(roadIndex, playerColor, game);
    }
    else {
        std :: cout << "Invalid settlement placement, cancelled settlement building" << std :: endl;
    }
}

void Land::occupyNeighborsRoad(int roadIndex, int playerColor, Catan &game) {
    // Start update roads for shared-edge neighbor:
    auto& neighbors = game.getBoard().neighbors;
    

    // Neighbor's index is the neighbor that shares edge with current land
    int neighborIndex = (roadIndex-1+6)%6;

    // Neighbor's land index
    int landIndex = neighbors[number][neighborIndex];
    
    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared edge index at neighbor's land
        int sharedEdgeIndex = (neighborIndex+4)%6;

        // Update shared edge in neighbor's land
        game.getBoard().getLand(neighborIndex).roads[sharedEdgeIndex] = playerColor;
    }
}

void Land::upgradeToCity(int spotIndex, int playerColor, Catan &game) {
    if(spotIndex < 0 || spotIndex >= 6) {
        std :: cout << "Invalid spot index, cancelled city building" << std :: endl;
        return;
    }

    if(isValidCity(spotIndex, playerColor, game)) {
        cities[spotIndex] = playerColor;
        settlements[spotIndex] = 0;
        occupyNeighborsCity(spotIndex, playerColor, game);
    }
    else {
        std :: cout << "Invalid settlement placement, cancelled city building" << std :: endl;
    }
}

void Land::occupyNeighborsCity(int spotIndex, int playerColor, Catan &game){
    // Get current land's neighbors
    auto& neighbors = game.getBoard().neighbors;
    
    // Start update cities for first neighbor:

    // First neighbor's index is the neighbor that shares edge at one counter-clockwise from requested road
    int neighborIndex = (spotIndex-2+6)%6;

    // Get first neighbor's land index
    int landIndex = neighbors[number][neighborIndex];
    
    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared spot index at neighbor's land
        int sharedSpot = (spotIndex+2)%6;

        // Update neighbor's shared spot
        game.getBoard().getLand(neighborIndex).cities[sharedSpot] = playerColor;
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = 0;
    }

    // Start update cities for second neighbor:

    // Second neighbor's index is the neighbor that shares edge from requested road
    int neighborIndex = (neighborIndex + 1) % 6;

    // Get second neighbor's land index
    int landIndex = neighbors[number][neighborIndex];

    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared spot index at neighbor's land
        int sharedSpot = (spotIndex-2+6)%6;

        // Update neighbor's shared spot
        game.getBoard().getLand(neighborIndex).cities[sharedSpot] = playerColor;
        game.getBoard().getLand(neighborIndex).settlements[sharedSpot] = 0;
    }
}

bool Land::isValidCity(int spotIndex, int playerColor, Catan &game) const{
    if (settlements[spotIndex] == playerColor && cities[spotIndex] == 0) {
        return true;
    }
    return false;
}

bool Land::isValidRoad(int roadIndex, int playerColor, Catan &game) const {
    // Check if roadIndex is already occupied
    if (roads[roadIndex] != 0) {
        std::cout << "Invalid request: Road index " << roadIndex << " is already occupied. Cancelling road building." << std::endl;
        return false;
    }

   
    // Check adjacent settlements and cities at current land
    if (settlements[roadIndex] == playerColor || cities[roadIndex] == playerColor || 
        settlements[(roadIndex+1)%6] == playerColor || cities[(roadIndex+1)%6] == playerColor) {
         return true;
    }

    // Check adjacent roads at current land
    if (roads[(roadIndex-1)%6] == playerColor || roads[(roadIndex+1)%6] == playerColor) {
         return true;
    }

    // Check adjacent lands to current land
    auto& neighbors = game.getBoard().neighbors;
    
    // Start check for first neighbor:

    // First neighbor's index is the neighbor that shares edge at one counter-clockwise from requested road
    int neighborIndex = (roadIndex-2+6)%6;

    // Get first neighbor's land index
    int landIndex = neighbors[number][neighborIndex];
    
    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared edge index at neighbor's land
        int sharedEdgeIndex = (neighborIndex+4)%6;

        // Check roads at neighboring land adjacent to requested road
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex-1+6)%6] == playerColor) {
            return true;
        }
    }
        
    // Start check for second neighbor:

    // Second neighbor's index is the neighbor that shares edge from requested road
    int neighborIndex = (neighborIndex + 1) % 6;

    // Get second neighbor's land index
    int landIndex = neighbors[number][neighborIndex];

    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared edge index at neighbor's land
        int sharedEdgeIndex = (neighborIndex+4)%6;

        // Check roads at neighboring land adjacent to requested road
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex-1+6)%6] == playerColor || 
            game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex+1+6)%6] == playerColor) {
            return true;
        }
    }


    // Start check for third neighbor:

    // Third neighbor's index is the neighbor that shares edge from after requested road
    int neighborIndex = (neighborIndex + 1) % 6;

    // Get Third neighbor's land index
    int landIndex = neighbors[number][neighborIndex];

    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared edge index at neighbor's land
        int sharedEdgeIndex = (neighborIndex+4)%6;

        // Check roads at neighboring land adjacent to requested road
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex+1+6)%6] == playerColor) {
            return true;
        }
    }


    // If no adjacent settlements, cities, or roads are found, that are owned by current player - return false
    return false;
}


bool Land::isValidSettlement(int setIndex, int playerColor, Catan &game) const {
    // Check if spot is already occupied
    if (settlements[setIndex] != 0 || cities[setIndex] != 0) {
        std::cout << "Invalid request: Settlement index " << roadIndex << " is already occupied. Cancelling settlement building." << std::endl;
        return false;
    }

   
    // Check adjacent settlements and cities at current land
    if (settlements[(setIndex-1)%6] != 0 || cities[(setIndex-1)%6] != 0 || 
        settlements[(setIndex+1)%6] != 0 || cities[(setIndex+1)%6] != 0) {
         return false;
    }

    bool hasNearbyRoad = false;

    // Check adjacent roads at current land
    if (roads[(setIndex-1)%6] == playerColor || roads[(setIndex)%6] == playerColor) {
         hasNearbyRoad = true;
    }

    // Check adjacent lands to current land
    auto& neighbors = game.getBoard().neighbors;
    
    // Start check for first neighbor:

    // First neighbor's index is the neighbor that shares edge at one counter-clockwise from requested point
    int neighborIndex = (setIndex-2+6)%6;

    // Get first neighbor's land index
    int landIndex = neighbors[number][neighborIndex];
    
    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared edge index at neighbor's land
        int sharedEdgeIndex = (neighborIndex+4)%6;

        // Check roads at neighboring land adjacent to requested road
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex-1+6)%6] == playerColor) {
            hasNearbyRoad = true;
        }

        // Check settlements at neighboring land adjacent to requested road
        if (game.getBoard().getLand(neighborIndex).settlements[(sharedEdgeIndex-1+6)%6] != 0 ||
            game.getBoard().getLand(neighborIndex).cities[(sharedEdgeIndex-1+6)%6] != 0) {
            return false;
        }
    }
        
    // Start check for second neighbor:

    // Second neighbor's index is the neighbor that shares edge from requested road
    int neighborIndex = (neighborIndex + 1) % 6;

    // Get second neighbor's land index
    int landIndex = neighbors[number][neighborIndex];

    // Check if neighbor land exists
    if (landIndex != -1) {

        // Get shared edge index at neighbor's land
        int sharedEdgeIndex = (neighborIndex+4)%6;

        // Check roads at neighboring land adjacent to requested road
        if (game.getBoard().getLand(neighborIndex).roads[(sharedEdgeIndex+1+6)%6] == playerColor) {
            hasNearbyRoad = true;
        }

        // Check settlements at neighboring land adjacent to requested road
        if (game.getBoard().getLand(neighborIndex).settlements[(sharedEdgeIndex+2+6)%6] != 0 ||
            game.getBoard().getLand(neighborIndex).cities[(sharedEdgeIndex+2+6)%6] != 0) {
            return false;
        }
    }


    // If no adjacent settlements, cities, and has adjacent road owned by player - return true
    return true && hasNearbyRoad;
}