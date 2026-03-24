#include "CoordsHolder.hpp"

void CoordsHolder::need(std::string name) {
    if (name == "X") x = reader.read(name);
    if (name == "Y") y = reader.read(name);
}

std::variant<int, std::string, bool> CoordsHolder::get(std::string name) {
    if (name == "X") return x;
    if (name == "Y") return y;
    return {};
}