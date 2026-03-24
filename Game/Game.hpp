#ifndef GAME_H
#define GAME_H

#include "../Abilities/AbilityManager.hpp"
#include "../Ships/ShipManager.hpp"
#include "../GameField/GameField.hpp"
#include <memory>

class GameState;

class Game {
public:
    Game();
    void startNewGame(std::vector<Length> ships, std::pair<int, int> fieldSize);
    bool attack(bool isPlayer, int x, int y);
    void enemyTurn();
    void useAbility(int x = -1, int y = -1);
    void endGame(bool playerWin);
    void nextRound();
    void placeShip(int index, Orientation orientation, int x, int y);
    void enemyPlaceShip();
    GameState& getGameState();
    
private:
    std::unique_ptr<GameState> state;
};

#endif GAME_H