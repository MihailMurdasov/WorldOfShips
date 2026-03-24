#ifndef ABILITYINTERFACE_H
#define ABILITYINTERFACE_H
#include <vector>
#include <unordered_map>
#include <string>

#include "ResultOfUsingAbilities.hpp"

class IAbility {
public:
    virtual void apply(ResultOfUsingAbilities& results) = 0;
    virtual ~IAbility() = default;
};

#endif ABILITYINTERFACE_H