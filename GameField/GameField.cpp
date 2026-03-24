#include "GameField.hpp"

GameField::GameField(int width, int height, ShipManager& manager)
    : Width(width), Height(height), field(height, std::vector<Cell>(width)), manager(manager) {
    checkFieldDimendions(width, height);
    clearField();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            field[i][j] = Cell();
        }
    }
}

GameField::GameField(const GameField& other)
    : Width(other.Width), Height(other.Height), field(other.field), manager(other.manager) {}

GameField::GameField(GameField&& other) noexcept
    : Width(other.Width), Height(other.Height), field(std::move(other.field)), manager(other.manager) {
    other.Width = 0;
    other.Height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        Width = other.Width;
        Height = other.Height;
        field = other.field;
        manager = other.manager;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        Width = other.Width;
        Height = other.Height;
        manager = other.manager;
        field = std::move(other.field);
        other.Width = 0;
        other.Height = 0;
    }
    return *this;
}

bool GameField::checkCoordsForPlacement(int x, int y, Length len, Orientation orientation, std::vector<std::vector<Cell>> field, bool isPlayer) const { 
    for (int i = 0; i < static_cast<int>(len); i++) {
        int posX = orientation == Orientation::Vertical ? x : x + i;
        int posY = orientation == Orientation::Vertical ? y + i : y;

        if (posX < 0 || posY < 0 || posX >= Width || posY >= Height) {
            if (isPlayer) {
                throw CoordsForPlacementException();
            } else {
                return false;
            }
        }
        if (field[posY][posX].getCellState() == CellState::Ship) {
            if (isPlayer) {
                throw CoordsForPlacementException();
            } else {
                return false;
            }
        }
    }
    int startX = x;
    int startY = y;
    int endX = orientation == Orientation::Vertical ? x : x + static_cast<int>(len)-1;
    int endY = orientation == Orientation::Vertical ? y + static_cast<int>(len)-1 : y;

    std::vector<std::pair<int,int>> badCells;

    if (orientation == Orientation::Vertical) {
        badCells.push_back(std::make_pair(startY - 1, x));
        badCells.push_back(std::make_pair(startY - 1, x - 1));
        badCells.push_back(std::make_pair(startY - 1, x + 1));
        badCells.push_back(std::make_pair(endY + 1, x));
        badCells.push_back(std::make_pair(endY + 1, x - 1));
        badCells.push_back(std::make_pair(endY + 1, x + 1));

        for (int i = 0; i < static_cast<int>(len); i++) {
            badCells.push_back(std::make_pair(startY + i, x + 1));
            badCells.push_back(std::make_pair(startY + i, x - 1));
        }
    } else {

        badCells.push_back(std::make_pair(y, startX - 1));
        badCells.push_back(std::make_pair(y - 1, startX - 1));
        badCells.push_back(std::make_pair(y + 1, startX - 1));
        badCells.push_back(std::make_pair(y, endX + 1));
        badCells.push_back(std::make_pair(y - 1, endX + 1));
        badCells.push_back(std::make_pair(y + 1, endX + 1));

        for (int i = 0; i < static_cast<int>(len); i++) {
            badCells.push_back(std::make_pair(y + 1, startX + i));
            badCells.push_back(std::make_pair(y - 1, startX + i));
        }
    }

    for(auto& coords : badCells) {
        if (coords.first >= 0 && coords.first < Height && coords.second >= 0 && coords.second < Width) {
            if (field[coords.first][coords.second].getCellState() == CellState::Ship) {
                if (isPlayer) {
                    throw CoordsForPlacementException();
                } else {
                    return false;
                }
            }
        }
    }
    return true;
}

void GameField::checkCoords(int x, int y) const {
    if (x < 0 || x >= Width || y < 0 || y >= Height) {
        throw CoordsException();
    }
}

void GameField::checkFieldDimendions(int width, int height) const{
    if (height < 0 || width < 0) {
        throw FieldDimendionsException();
    }
}

int GameField::getHeight() const {
    return Height;
}

int GameField::getWidth() const {
    return Width;
}

std::vector<std::vector<Cell>>& GameField::getField(){
    return field;
}

void GameField::clearField() {
    for (auto& row : field) {
        row.clear();
    }
    field.clear();
    std::vector<std::vector<Cell>> newField(Height, std::vector<Cell>(Width));
    field = newField;
}

bool GameField::placeShip(Ship& ship, Orientation orientation, int startX, int startY, bool isPlayer) {
    ship.setOrientation(orientation);
    if (checkCoordsForPlacement(startX, startY, ship.getLength(), orientation, field, isPlayer) && ship.getPlacedStatus() == false) {
        for (int i = 0; i < static_cast<int>(ship.getLength()); i++) {
            int posX = ship.getOrientation() == Orientation::Vertical ? startX : startX + i;
            int posY = ship.getOrientation() == Orientation::Vertical ? startY + i : startY;

            field[posY][posX].setCellState(CellState::Ship);
            field[posY][posX].setSegmentIndex(i);
            field[posY][posX].setShipPtr(&ship);
            field[posY][posX].setShipId(ship.getShipId());
        }
    } else {
        return false;
    }

    ship.setPlacedStatus(true);
    return true;
}

bool GameField::attackCell(int x, int y, ResultOfUsingAbilities& results){
    checkCoords(x, y);
    if (field[y][x].getCellState() == CellState::Ship) {
        if (field[y][x].getShipPtr()->isDestroyed()) {
            results.setDoubleDamageResult(DoubleDamageResult::DEACTIVATED);
            return false;
        } else {
            if (results.getDoubleDamageResult() == DoubleDamageResult::ACTIVATED) {
                field[y][x].getShipPtr()->hitSegment(field[y][x].getSegmentIndex(), 2);
                results.setDoubleDamageResult(DoubleDamageResult::DEACTIVATED);
            } else {
                field[y][x].getShipPtr()->hitSegment(field[y][x].getSegmentIndex(), 1);
            }
            field[y][x].setAttacked(true);
            if (field[y][x].getShipPtr()->isDestroyed()) {
                return true;
            }
        }
    } else {
        results.setDoubleDamageResult(DoubleDamageResult::DEACTIVATED);
        field[y][x].setCellState(CellState::Empty);
        field[y][x].setAttacked(true);
        return false;
    }

    return false;
}

Cell& GameField::getCell(int x, int y) {
    return field[y][x];
}

ShipManager& GameField::getManager() {
    return manager;
}

nlohmann::json GameField::toJson() {
    nlohmann::json jGameField = nlohmann::json::array();
    nlohmann::json shipsData = nlohmann::json::array();

    for (int y = 0; y < Height; y++) {
        nlohmann::json jRow = nlohmann::json::array();

        for (int x = 0; x < Width; x++) {
            jRow.push_back((field[y][x]).toJson());

            if (field[y][x].getCellState() == CellState::Ship) {
                if ((field[y][x]).getSegmentIndex() == 0) {
                    nlohmann::json shipData {
                        {"shipID", field[y][x].getShipPtr()->getShipId()},
                        {"len", static_cast<int>(field[y][x].getShipPtr()->getLength())},
                        {"x", x},
                        {"y", y},
                        {"orientation", static_cast<int>(field[y][x].getShipPtr()->getOrientation())}
                    };
                    shipsData.push_back(shipData);
                }
            }
        }
        jGameField.push_back(jRow);
    }

    return nlohmann::json{
        {"width", Width},
        {"height", Height},
        {"field", jGameField},
        {"ship_data", shipsData}
    };
}


void GameField::fromJson(nlohmann::json& j) {
    Width = j.at("width").get<int>();
    Height = j.at("height").get<int>();

    field.clear();
    field = std::vector<std::vector<Cell>>(Height, std::vector<Cell>(Width));
    for (int row = 0; row < Height; row++) {
        for (int cell = 0; cell < Width; cell++) {
            field[row][cell] = Cell();
        }
    }

    for (const auto& ship : j["ship_data"]) {
        placeShip(manager.getShips()[ship.at("shipID").get<int>()], static_cast<Orientation>(ship["orientation"]), ship["x"], ship["y"], true);
    }

    for (int row = 0; row < Height; row++) {
        for (int cell = 0; cell < Width; cell++) {
            field[row][cell].fromJson(j["field"].at(row).at(cell));
        }
    }
}