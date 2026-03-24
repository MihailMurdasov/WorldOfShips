#include "ConsoleRenderer.hpp"
#include <iostream>

Renderer::Renderer() {}

void Renderer::displayField(GameField& field, bool isEnemyField, int cursorX, int cursorY, int XLine, int YLine) {
    if (!isEnemyField) {
        std::cout << "-------------------------- ВАШЕ ПОЛЕ --------------------------\n";
    } else {
        std::cout << "----------------------- ВРАЖЕСКОЕ ПОЛЕ ------------------------\n";
    }
    selectedCells.clear(); 

    for (int y = cursorY; y < cursorY + YLine; y++) {
        for (int x = cursorX; x < cursorX + XLine; x++) {
            selectedCells.push_back(std::make_pair(x, y));
        }
    }

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            switch (field.getCell(x, y).getCellState()) {
                case CellState::Unknown:
                    if (isSelectCell(x, y) == true) {
                        printCell("?", true);
                    } else {
                        printCell("?", false);
                    }
                    break;
                case CellState::Empty:
                    if (isSelectCell(x, y) == true) {
                        printCell("empty", true);
                    } else {
                        printCell("empty", false);
                    }
                    break;
                case CellState::Ship:
                    if (isEnemyField) {
                        if (field.getCell(x,y).getAttacked()) {
                            int damaged = static_cast<int>(field.getCell(x, y).getShipPtr()->getSegmentState(field.getCell(x, y).getSegmentIndex()));
                            switch (damaged) {
                                case 0:
                                    if (isSelectCell(x, y) == true) {
                                        printCell("2", true);
                                    } else {
                                        printCell("2", false);
                                    }
                                    break;
                                case 1:
                                    if (isSelectCell(x, y) == true) {
                                        printCell("1", true);
                                    } else {
                                        printCell("1", false);
                                    }                                    
                                    break;
                                case 2:
                                    if (isSelectCell(x, y) == true) {
                                        printCell("X", true);
                                    } else {
                                        printCell("X", false);
                                    }                                    
                                    break;
                            }
                        } else {
                            if (isSelectCell(x, y) == true) {
                                printCell("?", true);
                            } else {
                                printCell("?", false);
                            }   
                        }   
                    } else {
                        int damaged = static_cast<int>(field.getCell(x,y).getShipPtr()->getSegmentState(field.getCell(x, y).getSegmentIndex()));
                        switch (damaged) {
                            case 0:
                                if (isSelectCell(x, y) == true) {
                                    printCell("2", true);
                                } else {
                                    printCell("2", false);
                                }
                                break;
                            case 1:
                                if (isSelectCell(x, y) == true) {
                                    printCell("1", true);
                                } else {
                                    printCell("1", false);
                                }                                    
                                break;
                            case 2:
                                if (isSelectCell(x, y) == true) {
                                    printCell("X", true);
                                } else {
                                    printCell("X", false);
                                }                                    
                                break;
                        }
                    }
                    break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------------------------------------------\n\n" << "\033[s";
}

void Renderer::printCell(std::string cell, bool select) const{
    std::string selectColor = "\033[1;97m";
    if (cell == "?") {
        if (select) {
            std::cout << selectColor << "[?] " << "\033[0m";
        } else {
            std::cout << "\033[1;34m" << "[?] " << "\033[0m";
        }
    } else if (cell == "X") {
        if (select) {
            std::cout << selectColor << "[X] " << "\033[0m";
        } else {
            std::cout << "\033[1;30m" << "[X] " << "\033[0m";
        }
    } else if (cell == "1") {
        if (select) {
            std::cout << selectColor << "[1] " << "\033[0m";
        } else {
            std::cout << "\033[1;31m" << "[1] " << "\033[0m";
        }
    } else if (cell == "2") {
        if (select) {
            std::cout << selectColor << "[2] " << "\033[0m";
        } else {
            std::cout << "\033[1;31m" << "[2] " << "\033[0m";
        }
    } else if (cell == "empty") {
        if (select) {
            std::cout << selectColor << "[\u25A0] " << "\033[0m";
        } else {
            std::cout << "\033[1;34m" << "[" << "\033[0;34m" << "\u25A0" << "\033[1;34m" << "] " << "\033[0m";
        }
    }
}

void Renderer::renderStaticInterface(bool isPlayerField) {
    std::cout << " __    __           _     _     ___  __   __ _     _           \n/ / /\\ \\ \\___  _ __| | __| |   /___\\/ _| / _\\ |__ (_)_ __  ___ \n\\ \\/  \\/ / _ \\| '__| |/ _` |  //  // |_  \\ \\| '_ \\| | '_ \\/ __|\n \\  /\\  / (_) | |  | | (_| | / \\_//|  _| _\\ \\ | | | | |_) \\__ \\\n  \\/  \\/ \\___/|_|  |_|\\__,_| \\___/ |_|   \\__/_| |_|_| .__/|___/\n                                                    |_|        \n\n" << "\033[s";
}

void Renderer::renderDialog(const std::string& message, bool temp, int durationSeconds) {
    std::cout << "\033[u";

    size_t dialogueHeight = lastMessageLines.size();

    for (size_t i = 0; i < dialogueHeight; ++i) {
        if (i < dialogueHeight - 1) {
            std::cout << "\033[B";
        }
    }
    for (size_t i = 0; i < dialogueHeight; ++i) {
        std::cout << "\033[2K";

        if (i < dialogueHeight - 1) {
            std::cout << "\033[A";
        }
    }
    std::vector<std::string> messageLines = splitMessageIntoLines(message);

    for (const std::string& line : messageLines) {
        std::cout << line << std::endl;
    }

    if (temp) {
        std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));
        tcflush(STDIN_FILENO, TCIFLUSH);

        dialogueHeight = messageLines.size();
        for (size_t i = 0; i < dialogueHeight; ++i) {
            if (i < dialogueHeight - 1) {
                std::cout << "\033[B";
            }
        }
        for (size_t i = 0; i < dialogueHeight; ++i) {
            std::cout << "\033[2K";

            if (i < dialogueHeight - 1) {
                std::cout << "\033[A";
            }
        }
        std::cout.flush();
    } else {
        lastMessageLines = messageLines;
    }

}

std::vector<std::string> Renderer::splitMessageIntoLines(const std::string& message) {
    std::vector<std::string> lines;
    std::istringstream stream(message);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

void Renderer::clearPreviousMessages() {
    if (lastMessageLines.empty()) return;
    
    size_t numLinesToClear = lastMessageLines.size();
    std::cout << "\033[999;999H";
    for (size_t i = 0; i < numLinesToClear; ++i) {
        std::cout << "\033[A\033[2K";
    }
}

bool Renderer::isSelectCell(int x, int y) {
    for (auto& cell : selectedCells) {
        if (cell.first == x && cell.second == y) return true;
    }
    return false;
}

void Renderer::render(Game& game, bool isEnemyField, bool haveField, int cursorX, int cursorY, int XLine, int YLine) {
    system("clear");
    renderStaticInterface(isEnemyField);

    if (isEnemyField && haveField) displayField(game.getGameState().getEnemyField(), isEnemyField, cursorX, cursorY, XLine, YLine);
    if (!isEnemyField && haveField) displayField(game.getGameState().getPlayerField(), isEnemyField, cursorX, cursorY, XLine, YLine);
}