#include "ShipManager.hpp"

ShipManager::ShipManager(int numberOfShips, std::vector<Length> lengthsOfShips) : numberOfShips(numberOfShips), ships(){
    ships.clear();
    for (int i = 0; i < numberOfShips; ++i) {
        ships.emplace_back(Ship(lengthsOfShips[i]));
        ships.back().setShipId(i);
    }
}

void ShipManager::clearManager() {
    ships.clear();
}

std::vector<Ship>& ShipManager::getShips() {
    return ships;
}

bool ShipManager::allShipsDestroyed() const {
    for (const auto& ship : ships) {
        if (!ship.isDestroyed()) {
            return false;
        }
    }
    return true;
}

nlohmann::json ShipManager::toJson () {
    nlohmann::json sizes = nlohmann::json::array();

    for (int i = 0; i < numberOfShips; ++i) {
        sizes.push_back(static_cast<int>(ships[i].getLength()));
    }

    return nlohmann::json{
        {"number", numberOfShips},
        {"sizes", sizes}
    };
}

void ShipManager::fromJson (nlohmann::json& j) {
    numberOfShips = j.at("number").get<int>();
    std::vector<Length> sizes = j.at("sizes").get<std::vector<Length>>();
    clearManager();

    for (int i = 0; i < numberOfShips; ++i) {
        ships.emplace_back(Ship(sizes[i]));
        ships.back().setShipId(i);
    }
}