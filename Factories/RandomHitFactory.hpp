#ifndef RANDOM_HIT_FACTORY_H
#define RANDOM_HIT_FACTORY_H

#include "IFactory.hpp"
#include "../Abilities/RandomHitAbility.hpp"

class RandomHitFactory : public IFactory {
public:
    RandomHitFactory(GameField& field);
    IAbility* createAbility(int x = -1, int y = -1);
    nlohmann::json toJson();
    std::string getInfo() const;
private:
    GameField& field;
};

#endif RANDOM_HIT_FACTORY_H