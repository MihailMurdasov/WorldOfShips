#include "Game.hpp"
#include "GameState.hpp"

Game::Game() : state(std::make_unique<GameState>()) {}

void Game::startNewGame(std::vector<Length> ships, std::pair<int, int> fieldSize){
    state->initialize(static_cast<int>(ships.size()), ships, fieldSize.first, fieldSize.second, false);
}

void Game::placeShip(int index, Orientation orientation, int x, int y) {
    state->getPlayerField().placeShip(state->getPlayerShipManager().getShips()[index], orientation, x, y, true);
}

void Game::enemyPlaceShip() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> randomWidth(0,state->getWidth()-1);
    std::uniform_int_distribution<> randomHeight(0,state->getHeight()-1);
    std::uniform_int_distribution<> randomOrientation(0,1);

    for (auto& ship : state->getEnemyShipManager().getShips()) {
        if (ship.getPlacedStatus() == false) {
            int randomX = randomWidth(gen);
            int randomY = randomHeight(gen);
            Orientation orientation = (Orientation)randomOrientation(gen);
            while(state->getEnemyField().placeShip(ship, orientation, randomX, randomY, false) != true) {
                randomX = randomWidth(gen);
                randomY = randomHeight(gen);
                orientation = (Orientation)randomOrientation(gen); 
            }
        }
    }
}

bool Game::attack(bool isPlayer, int x, int y) {
    if (isPlayer) {
        if (state->getEnemyField().attackCell(x, y, state->getResultOfUsingAbilities()) == true) {
            state->getAbilityManager().addRandomAbility();
            return true;
        } else {
            return false;
        }
    } else {
        state->getEnemyResultOfUsingAbilities().setDoubleDamageResult(DoubleDamageResult::DEACTIVATED);
        state->getPlayerField().attackCell(x, y, state->getEnemyResultOfUsingAbilities());
        return false;
    }
}

void Game::useAbility(int x, int y) {
    state->getAbilityManager().useAbility(state->getResultOfUsingAbilities(), x, y);
}

void Game::endGame(bool playerWin) {
    if (playerWin) {
        nextRound();
    } else {
        startNewGame(state->getPlayerShipsLength(), std::make_pair<int, int>(state->getHeight(), state->getWidth()));
    }
}

void Game::nextRound() {
    state->initialize(state->getPlayerShipManager().getShips().size(), state->getPlayerShipsLength(), state->getHeight(), state->getWidth(), true);
} 

void Game::enemyTurn() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> randomX(0,state->getPlayerField().getWidth()-1);
    std::uniform_int_distribution<> randomY(0,state->getPlayerField().getHeight()-1);
    int x = randomX(gen);
    int y = randomY(gen);
    while (state->getPlayerField().getField()[y][x].getCellState() == CellState::Empty) {
        x = randomX(gen);
        y = randomY(gen);
    }

    attack(false, x, y);
}

GameState& Game::getGameState() {
    return *state;
}