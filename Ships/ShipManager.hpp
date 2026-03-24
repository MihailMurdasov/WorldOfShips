#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H
#include "Ship.hpp"
#include "../Game/json.hpp"


class ShipManager {
public:
    ShipManager(int numberOfShips, std::vector<Length> lengthsOfShips);
    std::vector<Ship>& getShips();
    bool allShipsDestroyed() const;
    void clearManager();
    nlohmann::json toJson();
    void fromJson(nlohmann::json& j);
    
private:
    std::vector<Ship> ships;
    int numberOfShips;
};

#endif SHIPMANAGER_H