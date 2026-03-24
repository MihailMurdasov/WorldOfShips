#ifndef ABILITYMANAGER_H
#define ABILITYMANAGER_H

#include <queue>
#include <vector>
#include <random>
#include <algorithm>
#include <stack>

#include "IAbility.hpp"
#include "../Factories/IFactory.hpp"
#include "DoubleDamageAbility.hpp"
#include "ScannerAbility.hpp"
#include "RandomHitAbility.hpp"

#include "../Factories/DoubleDamageFactory.hpp"
#include "../Factories/RandomHitFactory.hpp"
#include "../Factories/ScannerFactory.hpp"

#include "../GameField/GameField.hpp"
#include "../Exceptions/NoAbilitiesException.hpp"

#include "ResultOfUsingAbilities.hpp"

#include "../Game/json.hpp"

class GameField;

class AbilityManager {
public:
    AbilityManager(GameField& field);
    AbilityManager(const AbilityManager& other);
    AbilityManager& operator=(const AbilityManager& other);
    void addRandomAbility();
    void useAbility(ResultOfUsingAbilities& results, int x = -1, int y = -1);
    std::string availableAbilities();
    std::string getNextAbility();
    std::string getNewAbility();
    bool checkAbilityAmount();
    nlohmann::json toJson();
    void fromJson(nlohmann::json& j);

private:
    GameField& field;
    std::queue<IFactory*> abilityQueue;

    void getStarterAbility();
};

#endif ABILITYMANAGER_H