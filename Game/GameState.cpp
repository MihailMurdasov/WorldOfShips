#include "GameState.hpp"

void GameState::initialize(int numbersOfShips, const std::vector<Length>& ships, int Height, int Width, bool skipPlayer) {
    this->Height = Height;
    this->Width = Width;

    enemyShipManager.reset();
    enemyField.reset();
    enemyResult = std::make_unique<ResultOfUsingAbilities>();

    enemyShipsLength = ships;
    enemyShipManager = std::make_unique<ShipManager>(numbersOfShips, enemyShipsLength);
    enemyField = std::make_unique<GameField>(Width, Height, *enemyShipManager);

    if (!skipPlayer) {
        playerShipsLength = ships;
        playerShipManager = std::make_unique<ShipManager>(numbersOfShips, playerShipsLength);
        playerField = std::make_unique<GameField>(Width, Height, *playerShipManager);
        result = std::make_unique<ResultOfUsingAbilities>();
        coordsHolder = std::make_unique<CoordsHolder>();
        abilityManager = std::make_unique<AbilityManager>(*enemyField);
    }
   
}

GameField& GameState::getPlayerField() {
    return *playerField;
}

GameField& GameState::getEnemyField() {
    return *enemyField;
}

ShipManager& GameState::getPlayerShipManager() {
    return *playerShipManager;
}

ShipManager& GameState::getEnemyShipManager() {
    return *enemyShipManager;
}

AbilityManager& GameState::getAbilityManager() {
    return *abilityManager;
}

ResultOfUsingAbilities& GameState::getResultOfUsingAbilities() {
    return *result;
}

ResultOfUsingAbilities& GameState::getEnemyResultOfUsingAbilities() {
    return *enemyResult;
}

std::vector<Length> GameState::getPlayerShipsLength() {
    return playerShipsLength;
}

std::vector<Length> GameState::getEnemyShipsLength() {
    return enemyShipsLength;
}

int GameState::getHeight() {
    return Height;
}

int GameState::getWidth() {
    return Width;
}

nlohmann::json GameState::toJson() const {
    return nlohmann::json{
        {"player_ship_manager", playerShipManager->toJson()},
        {"enemy_ship_manager", enemyShipManager->toJson()},
        {"player_field", playerField->toJson()},
        {"enemy_field", enemyField->toJson()},
        {"ability_manager", abilityManager->toJson()}

    };
}

void GameState::fromJson (nlohmann::json& j) {
    if (!playerShipManager) {
        playerShipManager = std::make_unique<ShipManager>(j["player_ship_manager"]["number"].get<int>(), j["player_ship_manager"]["sizes"].get<std::vector<Length>>());
    }
    if (!enemyShipManager) {
        enemyShipManager = std::make_unique<ShipManager>(j["enemy_ship_manager"]["number"].get<int>(), j["enemy_ship_manager"]["sizes"].get<std::vector<Length>>());
    }
    if (!playerField) {
        playerField = std::make_unique<GameField>(j["player_field"]["width"].get<int>(), j["player_field"]["height"].get<int>(), *playerShipManager);
    }
    if (!enemyField) {
        enemyField = std::make_unique<GameField>(j["enemy_field"]["width"].get<int>(), j["enemy_field"]["height"].get<int>(), *enemyShipManager);
    }
    if (!abilityManager) {
        abilityManager = std::make_unique<AbilityManager>(*enemyField);
    }
    if (!result) {
        result = std::make_unique<ResultOfUsingAbilities>();
    }
    if (!enemyResult) {
        enemyResult = std::make_unique<ResultOfUsingAbilities>();
    }

    Height = j["player_field"]["height"].get<int>();
    Width = j["player_field"]["width"].get<int>();
    playerShipManager->fromJson(j["player_ship_manager"]);
    enemyShipManager->fromJson(j["enemy_ship_manager"]);
    enemyField->fromJson(j["enemy_field"]);
    playerField->fromJson(j["player_field"]);
    abilityManager->fromJson(j["ability_manager"]);
}


std::istream& operator>>(std::istream& is, GameState& state) {
    /*
    std::string encryptedData;
    is >> encryptedData;

    std::string decryptedData = state.decryptData(encryptedData, state.getAESKey());
    std::cout << "Decrypted Data: " << decryptedData << std::endl;
    */

    nlohmann::json jsonData;
    is >> jsonData;
    /*
    try {
        jsonData = nlohmann::json::parse(decryptedData);
    } catch (...) {
        std::cerr << "JSON parse error: " << std::endl;
        throw OpenLoadFileException();
    }
    */
    
    nlohmann::json metadata = jsonData["metadata"];
    std::string savedHash = metadata["hash"];
    std::string savedHMAC = metadata["hmac"];
    size_t savedSize = metadata["size"];
    time_t timestamp = metadata["timestamp"];

    metadata.erase("hash");
    jsonData["metadata"] = metadata;

    if (state.calculateHash(jsonData.dump()) != savedHash) {
        throw std::runtime_error("Hash mismatch!");
    }

    jsonData.erase("metadata");

    if (state.generateHMAC(jsonData.dump(), state.getHmac()) != savedHMAC) {
        throw std::runtime_error("HMAC mismatch!");
    }

    if (jsonData.dump().size() != savedSize) {
        throw std::runtime_error("Size mismatch!");
    }


    state.fromJson(jsonData);

    return is;
}


std::ostream& operator<<(std::ostream& os, GameState& state) {
    nlohmann::json jsonData = state.toJson();
    std::string hmac = state.generateHMAC(jsonData.dump(), state.getHmac());

    nlohmann::json metadata;
    metadata["hmac"] = hmac;
    metadata["timestamp"] = time(nullptr);
    metadata["size"] = jsonData.dump().size();

    jsonData["metadata"] = metadata;

    std::string fileHash = state.calculateHash(jsonData.dump());

    jsonData["metadata"]["hash"] = fileHash;

    std::string serializedData = jsonData.dump(4);

    //std::string encryptedData = state.encryptData(serializedData, state.getAESKey());

    os << serializedData;

    return os;
}

void GameState::save(const std::string& name) {
    if (name.substr(name.find_last_of(".") + 1) != "json") {
        throw JSONException();
    }

    std::ofstream file(name);

    if (chmod(name.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) != 0) {
        throw std::runtime_error("Failed to remove read-only flag");
    }

    if (!file.is_open()) {
        throw OpenSaveFileException();
    }

    try {
        file << *this;
    } catch (...) {
        throw SaveFileException();
    }

    file.close();

    if (chmod(name.c_str(), S_IRUSR | S_IRGRP | S_IROTH) != 0) {
        throw std::runtime_error("Failed to set file as read-only");
    }
}

void GameState::load(const std::string& name) {
    if (name.substr(name.find_last_of(".") + 1) != "json") {
        throw JSONException();
    }

    std::ifstream file(name);

    if (!file.is_open()) {
        throw OpenLoadFileException();
    }

    try {
        file >> *this;
    } catch (...) {
        throw LoadFileException();
    }

    file.close();
}

std::string GameState::calculateHash(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    std::ostringstream os;
    for (unsigned char c : hash) {
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return os.str();
}

std::string GameState::generateHMAC(const std::string& data, const std::string& key) {
    unsigned char* result;
    unsigned int len = 32;
    result = HMAC(EVP_sha256(),
                  key.c_str(), key.size(),
                  reinterpret_cast<const unsigned char*>(data.c_str()), data.size(),
                  nullptr, nullptr);

    std::ostringstream os;
    for (unsigned int i = 0; i < len; i++) {
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }
    return os.str();
}

const std::string GameState::getHmac() {
    return hmacKey;
}

/*
const std::string GameState::getAESKey() {
    return AESKey;
}
std::string GameState::encryptData(const std::string& data, const std::string& key) {
    if (key.size() != 32) {
        throw std::invalid_argument("Key must be 32 bytes for AES-256");
    }

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw std::runtime_error("Failed to generate IV");
    }

    AES_KEY aesKey;
    AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(key.c_str()), 256, &aesKey);

    std::string paddedData = data;
    size_t padding = AES_BLOCK_SIZE - (data.size() % AES_BLOCK_SIZE);
    paddedData.append(padding, static_cast<char>(padding));

    std::string encryptedData(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE);
    unsigned char buffer[AES_BLOCK_SIZE];

    for (size_t i = 0; i < paddedData.size(); i += AES_BLOCK_SIZE) {
        AES_cbc_encrypt(
            reinterpret_cast<const unsigned char*>(&paddedData[i]),
            buffer,
            AES_BLOCK_SIZE,
            &aesKey,
            iv,
            AES_ENCRYPT
        );
        encryptedData.append(reinterpret_cast<char*>(buffer), AES_BLOCK_SIZE);
    }

    return encryptedData;
}

std::string GameState::decryptData(const std::string& data, const std::string& key) {
    if (key.size() != 32) {
        throw std::invalid_argument("Key must be 32 bytes for AES-256");
    }

    if (data.size() < AES_BLOCK_SIZE) {
        throw std::invalid_argument("Invalid encrypted data");
    }

    unsigned char iv[AES_BLOCK_SIZE];
    std::memcpy(iv, data.data(), AES_BLOCK_SIZE);

    AES_KEY aesKey;
    AES_set_decrypt_key(reinterpret_cast<const unsigned char*>(key.c_str()), 256, &aesKey);

    std::string decryptedData;
    decryptedData.resize(data.size() - AES_BLOCK_SIZE);

    for (size_t i = AES_BLOCK_SIZE; i < data.size(); i += AES_BLOCK_SIZE) {
        AES_cbc_encrypt(
            reinterpret_cast<const unsigned char*>(&data[i]),
            reinterpret_cast<unsigned char*>(&decryptedData[i - AES_BLOCK_SIZE]),
            AES_BLOCK_SIZE,
            &aesKey,
            iv,
            AES_DECRYPT
        );
    }

    char padding = decryptedData.back();
    if (padding < 1 || padding > AES_BLOCK_SIZE) {
        throw std::runtime_error("Invalid padding in decrypted data");
    }
    decryptedData.resize(decryptedData.size() - padding);

    return decryptedData;
}
*/