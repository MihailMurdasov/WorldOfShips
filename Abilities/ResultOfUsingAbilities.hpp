#ifndef RESULT_OF_USING_ABILITIES_H
#define RESULT_OF_USING_ABILITIES_H

#include <iostream>
#include "../Game/json.hpp"

enum class DoubleDamageResult {
    ACTIVATED,
    DEACTIVATED
};

enum class ScannerResult {
    Found,
    NotFound,
    NoInformation
};

enum class RandomHitResult {
    MISS,
    HIT,
    KILL,
    NoInformation
};

enum class LastAbilityUsed {
    DoubleDamage,
    Scanner,
    RandomHit,
    None
};

class ResultOfUsingAbilities {
public:
    void setDoubleDamageResult(DoubleDamageResult result);
    void setScannerResult(ScannerResult result);
    void setRandomHitResult(RandomHitResult result);
    void setLastAbilityUsed(LastAbilityUsed last);
    DoubleDamageResult getDoubleDamageResult() const;
    ScannerResult getScannerResult() const;
    LastAbilityUsed getLastAbilityUsed() const;
    RandomHitResult getRandomHitResult() const;
    void printResultOfUsing();

private:
    LastAbilityUsed lastAbilityUsed = LastAbilityUsed::None;
    DoubleDamageResult DDResult = DoubleDamageResult::DEACTIVATED;
    ScannerResult ScanResult = ScannerResult::NoInformation;
    RandomHitResult RHresult = RandomHitResult::NoInformation;
};

#endif RESULT_OF_USING_ABILITIES_H