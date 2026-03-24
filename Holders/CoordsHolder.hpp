#ifndef COORDS_HOLDER_H
#define COORDS_HOLDER_H

#include "InfoHolder.hpp"
#include "CoordsReader.hpp"

class CoordsHolder : public InfoHolder {
public:
    void need(std::string name);
    std::variant<int, std::string, bool> get(std::string name);

private:
    int x = -1, y = -1;
    CoordsReader reader;
};

#endif COORDS_HOLDER_H