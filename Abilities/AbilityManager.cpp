#include "AbilityManager.hpp"


AbilityManager::AbilityManager(GameField& field) : field(field) {
    getStarterAbility();
}

AbilityManager::AbilityManager(const AbilityManager& other)
    : field(other.field), abilityQueue(other.abilityQueue) {}

AbilityManager& AbilityManager::operator=(const AbilityManager& other) {
    if (this != &other) {
        field = other.field;
        abilityQueue = other.abilityQueue;
    }
    return *this;
}

void AbilityManager::getStarterAbility(){
    std::vector<IFactory*> abilitiesFactory = {new DoubleDamageFactory(), new RandomHitFactory(field), new ScannerFactory(field)};
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(abilitiesFactory.begin(), abilitiesFactory.end(), rng);

    for (IFactory* abilityFactory : abilitiesFactory) {
        abilityQueue.push(abilityFactory);
    }
}

std::string AbilityManager::getNextAbility() {
    return abilityQueue.front()->getInfo();
}

std::string AbilityManager::getNewAbility() {
    return abilityQueue.back()->getInfo();
}

void AbilityManager::addRandomAbility(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomAbilitygen(0,2);
    int randomAbility = randomAbilitygen(gen);

    switch(randomAbility) {
        case 0:
            abilityQueue.push(new DoubleDamageFactory());
            break;
        case 1:
            abilityQueue.push(new ScannerFactory(field));
            break;
        case 2:
            abilityQueue.push(new RandomHitFactory(field));
            break;
    }
}

bool AbilityManager::checkAbilityAmount() {
    if (abilityQueue.empty()) {
        return false;
    }
    return true;
}

void AbilityManager::useAbility(ResultOfUsingAbilities& results, int x, int y){
    if (abilityQueue.empty()) {
        throw NoAbilitiesException();
    }
    IAbility* ability = abilityQueue.front()->createAbility(x, y);
    ability->apply(results);
    abilityQueue.pop();
    delete ability;
}

std::string AbilityManager::availableAbilities() {
    if (abilityQueue.empty()) {
        return "[ Нет доступных способностей ]";
    } else {
        std::string abilities;
        std::vector<std::string> abilitiesInfo;
        std::queue<IFactory*> tempQueue = abilityQueue;
        
        while (!tempQueue.empty()) {
            abilitiesInfo.push_back(tempQueue.front()->getInfo());
            tempQueue.pop();
        }

        for (const auto& info : abilitiesInfo) {
            if (!abilities.empty()) {
                abilities += ", ";
            }
            abilities += info;
        }

        return "Доступные способности: " + abilities;
    }
}

nlohmann::json AbilityManager::toJson() {
    nlohmann::json abilities = nlohmann::json::array();

    std::queue<IFactory*> copyQueue = abilityQueue;

    while (!copyQueue.empty()) {
        abilities.push_back(copyQueue.front()->toJson());
        copyQueue.pop();
    }

    return nlohmann::json{
        {"abilities", abilities}
    };
}

void AbilityManager::fromJson(nlohmann::json& j) {\

    while (!abilityQueue.empty()) {
        delete abilityQueue.front();
        abilityQueue.pop();
    }
    
    for (const auto& item : j["abilities"]) {
        if (item[0] == "double_damage") {
            abilityQueue.push( new DoubleDamageFactory() );
        } else if (item[0] == "random_hit") {
            abilityQueue.push( new RandomHitFactory(field) );
        } else if (item[0] == "scanner") {
            abilityQueue.push( new ScannerFactory(field) );
        }
    }
}