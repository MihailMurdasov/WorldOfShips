#include "DoubleDamageAbility.hpp"

void DoubleDamage::apply(ResultOfUsingAbilities& results) {
    results.setDoubleDamageResult(DoubleDamageResult::ACTIVATED);
    results.setLastAbilityUsed(LastAbilityUsed::DoubleDamage);
}