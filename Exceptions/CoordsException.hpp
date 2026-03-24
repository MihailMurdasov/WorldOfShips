#ifndef COORDS_EXCEPTION_H
#define COORDS_EXCEPTION_H

#include <exception>

class CoordsException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Некорректные координаты.\n";
    }
};

#endif COORDS_EXCEPTION_H