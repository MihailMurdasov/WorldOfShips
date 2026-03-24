#include "ScannerFactory.hpp"

ScannerFactory::ScannerFactory(GameField& field) : field(field) {}

IAbility* ScannerFactory::createAbility(int x, int y) {
    return new Scanner(field, x, y);
}

std::string ScannerFactory::getInfo() const {
    return "[Scanner]";
}

nlohmann::json ScannerFactory::toJson () {
    return nlohmann::json{"scanner"};
}