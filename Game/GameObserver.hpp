#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H
#include "Game.hpp"
#include "GameState.hpp"

template <typename Renderer>
class GameObserver {
public:
    GameObserver(Game& gameInstance, const Renderer& rendererInstance)
        : game(gameInstance), renderer(rendererInstance) {}

    void update(Game& game, bool isEnemyField, bool haveField, int cursorX = -1, int cursorY = -1, int XLine = 1, int YLine = 1) {
        renderer.render(game, isEnemyField, haveField, cursorX, cursorY, XLine, YLine);
    }

    void renderDialog(const std::string& message, bool temp = false, int durationSeconds = 2) {
        renderer.renderDialog(message, temp, durationSeconds);
    }

private:
    Game& game;
    Renderer renderer;
};

#endif GAME_OBSERVER_H