#include "DoubleDamageFactory.hpp"

IAbility* DoubleDamageFactory::createAbility(int x, int y) {
    return new DoubleDamage();
}

std::string DoubleDamageFactory::getInfo() const {
    return "[Double Damage]";
}

nlohmann::json DoubleDamageFactory::toJson() {
    return nlohmann::json{"double_damage"};
}