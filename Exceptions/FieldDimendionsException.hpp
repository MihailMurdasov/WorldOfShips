#ifndef COORDS_DIMENDIONS_EXCEPTION_H
#define COORDS_DIMENDIONS_EXCEPTION_H

#include <exception>

class FieldDimendionsException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Некорректные размеры поля.\n";
    }
};

#endif COORDS_DIMENDIONS_EXCEPTION_H