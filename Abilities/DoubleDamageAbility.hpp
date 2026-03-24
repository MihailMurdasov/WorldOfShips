#ifndef DOUBLEDAMAGE_H
#define DOUBLEDAMAGE_H

#include "IAbility.hpp"
#include "../GameField/GameField.hpp"

class DoubleDamage : public IAbility {
public:
    void apply(ResultOfUsingAbilities& results);
};

#endif DOUBLEDAMAGE_H