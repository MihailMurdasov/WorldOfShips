#ifndef RANDOMHIT_H
#define RANDOMHIT_H

#include "IAbility.hpp"
#include "../GameField/GameField.hpp"
#include <random>
#include <cstdlib>

class RandomHit : public IAbility {
public:
    RandomHit(GameField& field);
    void apply(ResultOfUsingAbilities& results);

private:
    GameField& field;
};

#endif