#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include "../Exceptions/ShipLengthException.hpp"
#include "../Exceptions/SegmentIndexException.hpp"

enum class SegmentState {
    Intact,
    Damaged,
    Destroyed
};

enum class Orientation {
    Horizontal,
    Vertical
};

enum class Length {
    Small = 1,
    Medium = 2,
    Large = 3,
    ExtraLarge = 4
};

class Ship {
public:
    Ship(Length length);
    void setOrientation(Orientation orientation);
    Length getLength() const;
    Orientation getOrientation() const;
    void hitSegment(int segmentIndex, int damage);
    void setSegmentState(int segState, int index);
    SegmentState getSegmentState(int segmentIndex) const;
    void setPlacedStatus(bool placed);
    bool getPlacedStatus() const;
    bool isDestroyed() const;
    void setShipId(int id);
    int getShipId() const;
    
private:
    Length length;
    Orientation orientation;
    std::vector<SegmentState> state;
    bool isPlaced;
    int shipId;

    void checkSegmentIndex(int segmentIndex) const;
    void checkLength(Length length) const;
};

#endif SHIP_H