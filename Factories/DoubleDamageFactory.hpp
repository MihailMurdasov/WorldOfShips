#ifndef DOUBLE_DAMAGE_FACTORY_H
#define DOUBLE_DAMAGE_FACTORY_H

#include "IFactory.hpp"
#include "../Abilities/DoubleDamageAbility.hpp"
#include "../Holders/CoordsHolder.hpp"

class DoubleDamageFactory : public IFactory {
public:
    IAbility* createAbility(int x = -1, int y = -1);
    std::string getInfo() const;
    nlohmann::json toJson();
};

#endif DOUBLE_DAMAGE_FACTORY_H