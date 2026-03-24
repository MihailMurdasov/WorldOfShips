#include <iostream>
#include "Game/Game.hpp"
#include "Game/GameState.hpp"
#include "Game/CommandHandler.hpp"
#include "Game/ConsoleRenderer.hpp"
#include "Game/GameController.hpp"
#include "Game/GameObserver.hpp"

int main() {

    CommandHandler cmd("config.json");
    Renderer render;
    Game game;
    GameObserver<Renderer> obs(game, render);
    GameController<CommandHandler> control(game, cmd, obs);
    
    control.run();

    return 0;
}
