#ifndef SCANNER_FACTORY_H
#define SCANNER_FACTORY_H

#include "IFactory.hpp"
#include "../Abilities/ScannerAbility.hpp"
#include "../Holders/CoordsHolder.hpp"



class ScannerFactory : public IFactory {
public:
    ScannerFactory(GameField& field);
    IAbility* createAbility(int x = -1, int y = -1);
    nlohmann::json toJson();
    std::string getInfo() const;
private:
    CoordsHolder coordsHolder;
    GameField& field;
};

#endif SCANNER_FACTORY_H