#ifndef CELL_H
#define CELL_H

#include "../Ships/Ship.hpp"
#include "../Game/json.hpp"

enum class CellState {
    Unknown,
    Empty,
    Ship
};

class Cell {
public:
    Cell();
    void setCellState(CellState newState);
    CellState getCellState() const;
    void setShipPtr(Ship* ship);
    Ship* getShipPtr() const;
    void setSegmentIndex(int);
    int getSegmentIndex() const;
    void setAttacked(bool attacked);
    bool getAttacked() const;
    void setShipId(int id);
    int getShipId() const;
    nlohmann::json toJson();
    void fromJson(nlohmann::json& j);
    
private:
    bool attacked;
    CellState state;
    Ship* shipPtr;
    int segmentIndex;
    int shipId;
};

#endif CELL_H