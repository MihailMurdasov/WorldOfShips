#ifndef COORDS_FOR_PLACEMENT_EXCEPTION_H
#define COORDS_FOR_PLACEMENT_EXCEPTION_H

#include <exception>

class CoordsForPlacementException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Некорректные координаты для размещения корабля.\n";
    }
};

#endif COORDS_FOR_PLACEMENT_EXCEPTION_H