#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../Game/Game.hpp"
#include "json.hpp"
#include <fstream>

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <random>

#include "../Exceptions/JSONexception.hpp"
#include "../Exceptions/LoadFileException.hpp"
#include "../Exceptions/OpenLoadFileException.hpp"
#include "../Exceptions/OpenSaveFileException.hpp"
#include "../Exceptions/SaveFileException.hpp"

class GameState {

public:
    GameState() = default;
    void initialize(int numbresOfShipsPlayer, const std::vector<Length>& playerShips, int Height, int Width, bool skipPlayer);
    friend std::istream& operator>>(std::istream& is, GameState& state);
    friend std::ostream& operator<<(std::ostream& os, GameState& state);
    nlohmann::json toJson() const;
    void fromJson(nlohmann::json& j);
    void save(const std::string& name);
    void load(const std::string& name);

    GameField& getPlayerField();
    GameField& getEnemyField();
    ShipManager& getPlayerShipManager();
    ShipManager& getEnemyShipManager();
    AbilityManager& getAbilityManager();
    ResultOfUsingAbilities& getResultOfUsingAbilities();
    ResultOfUsingAbilities& getEnemyResultOfUsingAbilities();
    std::vector<Length> getPlayerShipsLength();
    std::vector<Length> getEnemyShipsLength();
    int getHeight();
    int getWidth();

    const std::string getHmac();
    const std::string getAESKey();
    std::string calculateHash(const std::string& data);
    std::string generateHMAC(const std::string& data, const std::string& key);
    std::string decryptData(const std::string& data, const std::string& key);
    std::string encryptData(const std::string& data, const std::string& key);

private:
    std::unique_ptr<GameField> playerField;
    std::unique_ptr<GameField> enemyField;
    std::unique_ptr<ShipManager> playerShipManager;
    std::unique_ptr<ShipManager> enemyShipManager;
    std::unique_ptr<AbilityManager> abilityManager;
    std::unique_ptr<ResultOfUsingAbilities> result;
    std::unique_ptr<ResultOfUsingAbilities> enemyResult;
    std::unique_ptr<CoordsHolder> coordsHolder;
    std::vector<Length> playerShipsLength;
    std::vector<Length> enemyShipsLength;
    int Height, Width;

    std::string metadataHash;
    size_t metadataSize = 0;
    time_t metadataTimestamp = 0;

    const std::string encryptionKey;
    const std::string hmacKey = "d0f3f6e34a8b539a2ecfa7889c35e8d2a65f8dcb87e3e5384fc19b66a5b081ac";
    //const std::string AESKey = "9a3b8e2f5cba4693f82a743a7a827dfd4539e8d2b8a03462be6fe8597fa89d6a";
};


#endif GAME_STATE_H