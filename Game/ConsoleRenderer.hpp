#ifndef RENDERER_H
#define RENDERER_H

#include "GameState.hpp"
#include "Game.hpp"
#include "../GameField/GameField.hpp"
#include "../GameField/Cell.hpp"
#include <chrono>
#include <thread>
#include <sstream>
#include <fstream>
#include <termios.h>
#include <unistd.h>

class Renderer {
public:
    Renderer();
    void displayField(GameField& field, bool isEnemyField, int cursorX = -1, int cursorY = -1, int XLine = 0, int YLine = 0);
    void printCell(std::string cell, bool select) const;
    void render(Game& game, bool isEnemyField, bool haveField, int cursorX = -1, int cursorY = -1, int XLine = 0, int YLine = 0);
    void renderStaticInterface(bool isPlayerField);
    void renderDialog(const std::string& message, bool temp = false, int durationSeconds = 2);

private:
    std::string lastMessage;
    std::vector<std::string> lastMessageLines;
    std::vector<std::string> splitMessageIntoLines(const std::string& message);
    void clearPreviousMessages();
    bool isSelectCell(int currentX, int currentY);
    std::vector<std::pair<int, int>> selectedCells;
    struct termios original_tio;
};

#endif RENDERER_H
 