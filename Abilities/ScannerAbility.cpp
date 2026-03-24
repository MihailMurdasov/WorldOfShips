#include "ScannerAbility.hpp"

Scanner::Scanner(GameField& field, int x, int y) : field(field), x(x), y(y) {}

bool Scanner::checkScanCoords(int y, int x) {
    if (y < 0 || y >= field.getHeight() || x < 0 || x >= field.getWidth()) {
        return false;
    }
    return true;
}

void Scanner::apply(ResultOfUsingAbilities& results) {
    int failed_count = 0;
    for (int i = y; i < y + 2; i++) {
        for (int j = x; j < x + 2; j++) {
            if (checkScanCoords(i, j) == true) {
                if (field.getCell(j, i).getCellState() == CellState::Ship && field.getCell(j, i).getShipPtr()->isDestroyed() == false) {
                    results.setScannerResult(ScannerResult::Found);
                    results.setLastAbilityUsed(LastAbilityUsed::Scanner);
                    return;
                }
            } else {
                failed_count++;
            }
        }
    }

    if (failed_count == 4) throw ScanCoordsException();

    results.setScannerResult(ScannerResult::NotFound);
    results.setLastAbilityUsed(LastAbilityUsed::Scanner);
}