#ifndef IFACTORY_H
#define IFACTORY_H

#include <iostream>
#include "../GameField/GameField.hpp"
#include "../Abilities/IAbility.hpp"
#include "../Holders/InfoHolder.hpp"

class IFactory {
public:
    virtual IAbility* createAbility(int x = -1, int y = -1) = 0;
    virtual std::string getInfo() const = 0;
    virtual nlohmann::json toJson() = 0;
    virtual ~IFactory() = default;
};

#endif IFACTORY_H