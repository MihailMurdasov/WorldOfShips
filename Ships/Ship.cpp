#include "Ship.hpp"

Ship::Ship(Length length)
    : length(length), state(std::vector<SegmentState>(static_cast<int>(length), SegmentState::Intact)) {
    checkLength(length);
    setPlacedStatus(false);
}

void Ship::checkSegmentIndex(int segmentIndex) const {
    if (segmentIndex < 0 || segmentIndex > static_cast<int>(this->length)) {
        throw SegmentIndexException();
    }
}

void Ship::checkLength(Length length) const {
    if (static_cast<int>(length) < 0 || length > Length::ExtraLarge) {
        throw ShipLengthException();
    }
}

void Ship::setPlacedStatus(bool placed) {
    isPlaced = placed;
}

bool Ship::getPlacedStatus() const {
    return isPlaced;
}

void Ship::setOrientation(Orientation orientation) {
    this->orientation = orientation;
}

Length Ship::getLength() const {
    return length;
}

Orientation Ship::getOrientation() const {
    return orientation;
}

void Ship::setShipId(int id) {
    shipId = id;
}

int Ship::getShipId() const {
    return shipId;
}

void Ship::setSegmentState(int segState, int index) {
    switch (segState)
    {
    case 0:
        state[index] = SegmentState::Intact;
        break;

    case 1:
        state[index] = SegmentState::Damaged;
        break;

    case 2:
        state[index] = SegmentState::Destroyed;
        break;

    default:
        break;
    }
}

SegmentState Ship::getSegmentState(int segmentIndex) const {
    checkSegmentIndex(segmentIndex);
    return state[segmentIndex];
}

void Ship::hitSegment(int segmentIndex, int damage) {
    checkSegmentIndex(segmentIndex);

    for(int i = 0; i < damage; i++) {
        if (state[segmentIndex] == SegmentState::Destroyed) return;
        
        if (state[segmentIndex] == SegmentState::Intact) {
            state[segmentIndex] = SegmentState::Damaged;
        } else if (state[segmentIndex] == SegmentState::Damaged) {
            state[segmentIndex] = SegmentState::Destroyed;
        }
    }
}

bool Ship::isDestroyed() const {
    for (const auto& stateInfo : state) {
        if (stateInfo != SegmentState::Destroyed) {
            return false;
        }
    }
    return true;
}