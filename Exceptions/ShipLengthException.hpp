#ifndef SHIP_LENGTH_EXCEPTION_H
#define SHIP_LENGTH_EXCEPTION_H

#include <exception>

class ShipLengthException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Некорректная длина корабля.\n";
    }
};

#endif SHIP_LENGTH_EXCEPTION_H