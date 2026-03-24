#include "ResultOfUsingAbilities.hpp"

void ResultOfUsingAbilities::setDoubleDamageResult(DoubleDamageResult result) {
    DDResult = result;
}

void ResultOfUsingAbilities::setScannerResult(ScannerResult result) {
    ScanResult = result;
}

void ResultOfUsingAbilities::setLastAbilityUsed(LastAbilityUsed last) {
    lastAbilityUsed = last;
}

void ResultOfUsingAbilities::setRandomHitResult(RandomHitResult result) {
    RHresult = result;
}

RandomHitResult ResultOfUsingAbilities::getRandomHitResult() const {
    return RHresult;
}

DoubleDamageResult ResultOfUsingAbilities::getDoubleDamageResult() const {
    return DDResult;
}

ScannerResult ResultOfUsingAbilities::getScannerResult() const {
    return ScanResult;
}

LastAbilityUsed ResultOfUsingAbilities::getLastAbilityUsed() const {
    return lastAbilityUsed;
}

void ResultOfUsingAbilities::printResultOfUsing() {
    switch(lastAbilityUsed) {
        case LastAbilityUsed::DoubleDamage:
            std::cout << "[ Активировано ]: Double Damage" << std::endl;
            break;
        case LastAbilityUsed::Scanner:
            std::cout << "[ Активировано ]: Scanner" << std::endl;
            if (ScanResult == ScannerResult::Found) std::cout << "[ Результат ]: Корабль обнаружен" << std::endl;
            if (ScanResult == ScannerResult::NotFound) std::cout << "[ Результат ]: Корабль не найден" << std::endl;
            break;
        case LastAbilityUsed::RandomHit:
            std::cout << "[ Активировано ]: Random Hit" << std::endl;
            break;
    }
}
