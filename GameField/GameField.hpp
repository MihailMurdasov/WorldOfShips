#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <iostream>
#include <random>
#include <cstdlib>
#include <stdexcept>
#include <ctime>

#include "Cell.hpp"
#include "../Ships/ShipManager.hpp"
#include "../Exceptions/CoordsException.hpp"
#include "../Exceptions/CoordsForPlacementException.hpp"
#include "../Exceptions/FieldDimendionsException.hpp"

#include "../Abilities/ResultOfUsingAbilities.hpp"


class GameField {
public:
    GameField(int width, int height, ShipManager& manager);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    bool placeShip(Ship& ship, Orientation orientation, int startX, int startY, bool isPlayer);
    bool attackCell(int x, int y, ResultOfUsingAbilities& results);
    int getHeight() const;
    int getWidth() const;
    void clearField();
    std::vector<std::vector<Cell>>& getField();
    void randomPlacement();
    Cell& getCell(int x, int y);
    ShipManager& getManager();
    nlohmann::json toJson();
    void fromJson(nlohmann::json& j);


private:
    int Height;
    int Width;
    std::vector<std::vector<Cell>> field;
    ShipManager& manager;

    void checkCoords(int x, int y) const;
    void checkFieldDimendions(int width, int height) const;
    bool checkCoordsForPlacement(int x, int y, Length len, Orientation orientation, std::vector<std::vector<Cell>> field, bool isPlayer) const;
};

#endif GAMEFIELD_H