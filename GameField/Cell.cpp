#include "Cell.hpp"
#include <iostream>

Cell::Cell() : state(CellState::Unknown), shipPtr(nullptr), segmentIndex(-1), attacked(false), shipId(-1) {}

void Cell::setCellState(CellState newState) {
    state = static_cast<CellState>(newState);
}

CellState Cell::getCellState() const {
    return state;
}

Ship* Cell::getShipPtr() const {
    return shipPtr;
}

void Cell::setShipPtr(Ship* ship) {
    shipPtr = ship;
}

void Cell::setSegmentIndex(int ind) {
    segmentIndex = ind;
}

int Cell::getSegmentIndex() const {
    return segmentIndex;
}

void Cell::setShipId(int id) {
    shipId = id;
}

int Cell::getShipId() const {
    return shipId;
}

void Cell::setAttacked(bool attacked) {
    this->attacked = attacked;
}

bool Cell::getAttacked() const {
    return attacked;
}

nlohmann::json Cell::toJson() {
    std::string state;
    int segmentState = -1;
    if (this->state == CellState::Unknown) {
        state = "unknown";
    } else if (this->state == CellState::Empty) {
        state = "empty";
    } else {
        state = "ship";
    }

    if (segmentIndex != -1) {
        segmentState = static_cast<int>(shipPtr->getSegmentState(segmentIndex));
    }

    return nlohmann::json{
        {"attacked", attacked},
        {"state", state},
        {"ship_id", shipId},
        {"segment_id", segmentIndex},
        {"segment_state", segmentState}
    };
}


void Cell::fromJson(nlohmann::json& j) {
    attacked = j.at("attacked").get<bool>();
    if (j["state"] == "unknown") {
        state = CellState::Unknown;
    } else if (j["state"] == "empty") {
        state = CellState::Empty;
    } else {
        state = CellState::Ship;
    }
    shipId = j.at("ship_id").get<int>();
    segmentIndex = j.at("segment_id").get<int>();
    if (state == CellState::Ship) {
        shipPtr->setSegmentState(j.at("segment_state").get<int>(), segmentIndex);
    }
}