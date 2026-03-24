#ifndef SCANNER_H
#define SCANNER_H

#include "IAbility.hpp"
#include "../GameField/GameField.hpp"
#include "../Exceptions/ScanCoordsException.hpp"

class Scanner : public IAbility {
public:
    Scanner(GameField& field, int x, int y);
    void apply(ResultOfUsingAbilities& results);

private:
    bool checkScanCoords(int y, int x);
    GameField& field;
    int x, y;
};

#endif SCANNER_H