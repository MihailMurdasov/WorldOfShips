#include "RandomHitFactory.hpp"

RandomHitFactory::RandomHitFactory(GameField& field) : field(field) {}

IAbility* RandomHitFactory::createAbility(int x, int y) {
    return new RandomHit(field);
}

std::string RandomHitFactory::getInfo() const {
    return "[Random Hit]";
}

nlohmann::json RandomHitFactory::toJson() {
    return nlohmann::json{"random_hit"};
}