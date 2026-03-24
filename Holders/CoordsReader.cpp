#include "CoordsReader.hpp"

int CoordsReader::read(std::string name) {
    int parameter;
    std::cout << "Введите значение " + name + ": ";
    std::cin >> parameter;
    return parameter;
}