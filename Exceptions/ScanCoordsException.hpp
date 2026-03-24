#ifndef SCAN_COORDS_EXCEPTION_H
#define SCAN_COORDS_EXCEPTION_H

#include <exception>

class ScanCoordsException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Область сканирования выходит за рамки поля.\n";
    }
};

#endif SCAN_COORDS_EXCEPTION_H