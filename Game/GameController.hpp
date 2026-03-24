#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include "CommandHandler.hpp"
#include <iostream>
#include <unistd.h>
#include <cctype>
#include "Game.hpp"
#include "GameState.hpp"
#include "ConsoleRenderer.hpp"
#include "GameObserver.hpp"

template <typename CommandHandler>
class GameController {
public:
    GameController(Game& game, CommandHandler& handler, GameObserver<Renderer>& observerInstance)
    : game(game), commandHandler(handler), observer(observerInstance) {
        try {
            commandHandler.loadConfig();
            observer.update(game, false, false);
            observer.renderDialog("[Конфигурация управления загружена успешно]...\n", true, 2);
        } catch (ConfigFileException& e) {
            observer.update(game, false, false);
            observer.renderDialog(std::string(e.what()) + "[Управление будет задано по умолчанию]...", true, 2);
            commandHandler.loadDefaultMapping();
        } catch (LoadFileException& e) {
            observer.update(game, false, false);
            observer.renderDialog(std::string(e.what()) + "[Управление будет задано по умолчанию]...", true, 2);
            commandHandler.loadDefaultMapping();
        } catch (UnknownCommandException& e) {
            observer.update(game, false, false);
            observer.renderDialog(std::string(e.what()) + "[Управление будет задано по умолчанию]...", true, 2);
            commandHandler.loadDefaultMapping();
        } catch (SecondKeyException& e) {
            observer.update(game, false, false);
            observer.renderDialog(std::string(e.what()) + "[Управление будет задано по умолчанию]...", true, 2);
            commandHandler.loadDefaultMapping();
        } catch (SecondCommandException& e) {
            observer.update(game, false, false);
            observer.renderDialog(std::string(e.what()) + "[Управление будет задано по умолчанию]...", true, 2);
            commandHandler.loadDefaultMapping();
        } catch (NoKeyException& e) {
            observer.update(game, false, false);
            observer.renderDialog(std::string(e.what()) + "[Управление будет задано по умолчанию]...", true, 2);
            commandHandler.loadDefaultMapping();
        }
        map = commandHandler.getReverseKeyMapping();
    }

    void run(bool isContinue = false) {
        bool menuOn = true;
        Command command;
        std::vector<Length> ships;
        std::pair<int, int> fieldSize;
        std::string filename;
        bool loaded;
        while (menuOn) {
            observer.update(game, false, false);
            if (isContinue) {
                observer.renderDialog("[ ГЛАВНОЕ МЕНЮ ]\n[" + map[Command::PLAY] + "] Начать новую игру\n[" + map[Command::LOAD] + "] Загрузить сохранение\n[" + map[Command::EXIT] + "] Завершить игру (несохраненные данные будут утеряны)\n[" + map[Command::CONTINUE] + "] Продолжить игру\n\n");
            } else {
                observer.renderDialog("[ ГЛАВНОЕ МЕНЮ ]\n[" + map[Command::PLAY] + "] Начать новую игру\n[" + map[Command::LOAD] + "] Загрузить сохранение\n[" + map[Command::EXIT] + "] Завершить игру\n\n");
            }
            command = commandHandler.getCommand();
            switch (command) {
                case Command::PLAY:
                    ships = getShipSizesFromUser();
                    observer.renderDialog("[Корабли укомплектованы]", true, 2);
                    fieldSize = getFieldDimensionsFromUser();
                    game.startNewGame(ships, fieldSize);
                    observer.update(game, false, true);
                    observer.renderDialog("[Поле построено]", true, 2);
                    observer.renderDialog("[Фаза подготовки]", true, 2);
                    if (preparePhase() == false) run();
                    round();
                    break;
                case Command::LOAD:
                    loaded = false;
                    while (!loaded) {
                        try {
                            observer.renderDialog("Загрузить сохраненную игру?\n[" + map[Command::YES] + "] Да    [" + map[Command::NO] + "] Нет\n");
                            if (!confirm()) break;
                            observer.renderDialog("Введите имя файла для загрузки: ");
                            filename = commandHandler.getFileName();
                            game.getGameState().load(filename);
                            observer.renderDialog("[Игра успешно загружена из файла: " + filename + "]\n" + std::string(filename.length(), ' '), true, 2);
                            loaded = true;
                            round();
                            break;
                        } catch (JSONException& e) {
                            std::cin.clear();
                            observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                        } catch (OpenLoadFileException& e) {
                            observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                        } catch (LoadFileException& e) {
                            observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                        }
                    }
                    break;
                case Command::EXIT:
                    observer.renderDialog("Игра завершена. До свидания!");
                    if (isContinue) exit(0);
                    menuOn = false;
                    return;
                    break;  
                case Command::CONTINUE:
                    if (isContinue) {
                        menuOn = false;
                        break;
                    } else {
                        break;
                    }
                default:
                    break;                 
            }
        }
    }

    bool preparePhase(bool skipPlayer = false) {
        if (!skipPlayer) {
            for (int i = 0; i < game.getGameState().getPlayerShipManager().getShips().size(); ++i) {   
                bool placed = false;
                while (!placed) {
                    try {
                        observer.update(game, false, true);
                        std::vector<int> params = selectCellForShip("Разместите корабль №" + std::to_string(i + 1), game.getGameState().getPlayerShipsLength()[i], false);
                        if (params[0] == -1 && params[1] == -1 && params[2] == -1) return false;
                        game.placeShip(i, static_cast<Orientation>(params[2]), params[0], params[1]);
                        placed = true;
                        break;
                    } catch (CoordsForPlacementException& e) {
                        observer.renderDialog(e.what(), true, 2);
                    }
                }
            }
        }
        observer.update(game, true, true);
        observer.renderDialog("[Бот расставляет корабли...]\n", true, 3);
        game.enemyPlaceShip();
        return true;
    }

    void round() {
        observer.update(game, isEnemyField, true);
        observer.renderDialog("[ ВАШ ХОД ]\n[" + map[Command::ATTACK] + "] Атаковать ячейку\n[" + map[Command::USE_ABILITY] + "] Использовать способность\n[" + map[Command::FIELD_SWITCH] + "] Смена поля\n[" + map[Command::SHOW_ABILITIES] + "] Вывести доступные способности\n[" + map[Command::SAVE] + "] Сохранить игру\n[" + map[Command::LOAD] + "] Загрузить игру\n[" + map[Command::END_TURN] + "] Завершить ход\n [" + map[Command::MENU] + "] Выйти в меню\n");
        while (game.getGameState().getPlayerShipManager().allShipsDestroyed() && !game.getGameState().getEnemyShipManager().allShipsDestroyed() || !isGameOver) {
            observer.update(game, isEnemyField, true);
            observer.renderDialog("[ ВАШ ХОД ]\n[" + map[Command::ATTACK] + "] Атаковать ячейку\n[" + map[Command::USE_ABILITY] + "] Использовать способность\n[" + map[Command::FIELD_SWITCH] + "] Смена поля\n[" + map[Command::SHOW_ABILITIES] + "] Вывести доступные способности\n[" + map[Command::SAVE] + "] Сохранить игру\n[" + map[Command::LOAD] + "] Загрузить игру\n[" + map[Command::END_TURN] + "] Завершить ход\n[" + map[Command::MENU] + "] Выйти в меню\n");

            bool endTurn = false;
            bool attackMadeFlag = false;
            bool abilityUsedFlag = false;
            Command command;
            std::pair<int, int> coords;
            std::string filename;
            bool saved;
            bool loaded;
            while (!endTurn) {
                observer.update(game, isEnemyField, true);
                observer.renderDialog("[ ВАШ ХОД ]\n[" + map[Command::ATTACK] + "] Атаковать ячейку\n[" + map[Command::USE_ABILITY] + "] Использовать способность\n[" + map[Command::FIELD_SWITCH] + "] Смена поля\n[" + map[Command::SHOW_ABILITIES] + "] Вывести доступные способности\n[" + map[Command::SAVE] + "] Сохранить игру\n[" + map[Command::LOAD] + "] Загрузить игру\n[" + map[Command::END_TURN] + "] Завершить ход\n[" + map[Command::MENU] + "] Выйти в меню\n");
                command = commandHandler.getCommand();
                switch (command) {
                    case Command::ATTACK:
                        if (!attackMadeFlag) {
                            while (!attackMadeFlag) {
                                try {
                                    coords = selectCell("Выберите клетку для атаки...\n", true);
                                    if (coords != std::make_pair(-1, -1)) {
                                        SegmentState stateAfter = SegmentState::Intact;
                                        SegmentState stateBefore = SegmentState::Intact;
                                        if (game.getGameState().getEnemyField().getCell(coords.first, coords.second).getCellState() == CellState::Ship) {
                                            stateAfter = game.getGameState().getEnemyField().getCell(coords.first, coords.second).getShipPtr()->getSegmentState(game.getGameState().getEnemyField().getCell(coords.first, coords.second).getSegmentIndex());
                                        }
                                        bool attack = game.attack(true, coords.first, coords.second);
                                        if (game.getGameState().getEnemyField().getCell(coords.first, coords.second).getCellState() == CellState::Ship) {
                                            stateBefore = game.getGameState().getEnemyField().getCell(coords.first, coords.second).getShipPtr()->getSegmentState(game.getGameState().getEnemyField().getCell(coords.first, coords.second).getSegmentIndex());
                                        }              

                                        if (attack) {
                                            observer.update(game, true, true);
                                            observer.renderDialog("[Убит!]\n[Получена новая способность!]: " + game.getGameState().getAbilityManager().getNewAbility() + "\n", true,3);
                                        } else if (stateAfter != stateBefore) {
                                            observer.update(game, true, true);
                                            observer.renderDialog("[Есть попадание!]\n", true, 2);
                                        } else {
                                            observer.update(game, true, true);
                                            observer.renderDialog("[Промах!]\n", true, 2);        
                                        }
                                        attackMadeFlag = true;
                                        break;
                                    } else {
                                        break;
                                    }
                                } catch (CoordsException& e) {
                                    observer.renderDialog(e.what(), true, 2); 
                                }
                            }
                            if (game.getGameState().getEnemyShipManager().allShipsDestroyed()) {
                                endTurn = true;
                                break;
                            }
                        } else {
                            observer.renderDialog("Вы уже атаковали в этом ходе.", true, 2);
                        }
                        break;  
                    case Command::USE_ABILITY:
                        if (!abilityUsedFlag) {
                            std::string nextAbilityName;
                            if (game.getGameState().getAbilityManager().checkAbilityAmount()) {
                                nextAbilityName = game.getGameState().getAbilityManager().getNextAbility();
                                observer.renderDialog("Вы уверены, что хотите ипользовать способность " + nextAbilityName + "?\n[" + map[Command::YES] + "] Да    [" + map[Command::NO] + "] Нет\n");
                                if (!confirm()) break;
                            }
                            if (nextAbilityName == "[Scanner]") {
                                coords = selectCell("Выберите область сканирования...\n", true, 2, 2);
                                if (coords != std::make_pair(-1, -1)) {
                                    try {
                                        game.useAbility(coords.first, coords.second);
                                        abilityUsedFlag = true;
                                        std::string res;
                                        if (game.getGameState().getResultOfUsingAbilities().getScannerResult() == ScannerResult::Found) {
                                            res = "Обранужен корабль!";
                                        } else {
                                            res = "Ничего не найдено.";
                                        }
                                        observer.renderDialog("[Вы использовали способность]: " + nextAbilityName + "\n[Результат]: " + res , true, 3);
                                    } catch (NoAbilitiesException& e) {
                                        observer.renderDialog(e.what(), true, 2);
                                    } catch (ScanCoordsException& e) {
                                        observer.renderDialog(e.what(), true, 2);
                                    }
                                }
                            } else {
                                try {
                                    game.useAbility();
                                    std::string res;
                                    if (nextAbilityName == "[Double Damage]") res = "Следующая атака нанесет двойной урон!";
                                    if (nextAbilityName == "[Random Hit]") {
                                        if (game.getGameState().getResultOfUsingAbilities().getRandomHitResult() == RandomHitResult::KILL) {
                                            game.getGameState().getAbilityManager().addRandomAbility();
                                            res = "[Убит!]\n[Получена новая способность!]: " + game.getGameState().getAbilityManager().getNewAbility() + "\n"; 
                                        }
                                        if (game.getGameState().getResultOfUsingAbilities().getRandomHitResult() == RandomHitResult::HIT) res = "[Есть попадание!]\n";
                                    }
                                    observer.renderDialog("[Вы использовали способность]: " + nextAbilityName + "\n[Результат]: " + res, true, 3);
                                    abilityUsedFlag = true;
                                } catch (NoAbilitiesException& e) {
                                    observer.renderDialog(e.what(), true, 2);
                                }
                            }
                        } else {
                            observer.renderDialog("Вы уже использовали способность в этом ходе.", true, 2);
                        }
                        break;     
                    case Command::END_TURN:
                        endTurn = true;
                        break;     
                    case Command::SAVE:
                        saved = false;
                        while (!saved) {
                            try {
                                observer.renderDialog("Сохранить игру?\n[" + map[Command::YES] + "] Да    [" + map[Command::NO] + "] Нет\n");
                                if (!confirm()) break;
                                observer.renderDialog("Введите имя файла для сохранения: ");
                                filename = commandHandler.getFileName();
                                game.getGameState().save(filename);
                                observer.renderDialog("[Игра успешно сохранена в файл: " + filename + "]\n" + std::string(filename.length(), ' '), true, 2);
                                saved = true;
                                break;
                            } catch (JSONException& e) {
                                observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                            } catch (OpenLoadFileException& e) {
                                observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                            } catch (LoadFileException& e) {
                                observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                            }      
                        }
                        break;  
                    case Command::LOAD:
                        loaded = false;
                        while (!loaded) {
                            try {
                                observer.renderDialog("Загрузить сохраненную игру?\n[" + map[Command::YES] + "] Да    [" + map[Command::NO] + "] Нет\n");
                                if (!confirm()) break;
                                observer.renderDialog("Введите имя файла для загрузки: ");
                                filename = commandHandler.getFileName();
                                game.getGameState().load(filename);
                                observer.renderDialog("[Игра успешно загружена из файла: " + filename + "]\n" + std::string(filename.length(), ' '), true, 2);
                                loaded = true;
                                round();
                                break;
                            } catch (JSONException& e) {
                                observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                            } catch (OpenLoadFileException& e) {
                                observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                            } catch (LoadFileException& e) {
                                observer.renderDialog(e.what() + std::string(filename.length(), ' '), true, 2);
                            }
                        }
                        break; 
                    case Command::SHOW_ABILITIES:
                        observer.renderDialog(game.getGameState().getAbilityManager().availableAbilities() + "\n[" + map[Command::CANCEL] + "] - назад");
                        cancelInfo();
                        break;  
                    case Command::MENU:
                        run(true);
                        break;         
                    case Command::FIELD_SWITCH:
                        isEnemyField = !isEnemyField;
                        break;
                    default:
                        break;
                }
            }
            if (isGameOver) return;

            if (game.getGameState().getPlayerShipManager().allShipsDestroyed()) {
                observer.update(game, false, true);
                observer.renderDialog("[Поражение...]\n[Начинается новая игра...]\n", true, 3);
                game.endGame(false);
                isGameOver = false;
                if (preparePhase() == false) run();
                round();
                break;
            } else if (game.getGameState().getEnemyShipManager().allShipsDestroyed()) {
                observer.update(game, true, true);
                observer.renderDialog("[ПОБЕДА!!!]\n[Начинается новый раунд...]\n", true, 3);
                game.endGame(true);
                isGameOver = false;
                preparePhase(true);
                round();
                break;
            }

            observer.update(game, false, true);
            observer.renderDialog("[Противник делает ход...]\n", true, 1);
            game.enemyTurn();
            observer.update(game, false, true);
            observer.renderDialog("[Противник делает ход...]\n", true, 2);

            if (game.getGameState().getPlayerShipManager().allShipsDestroyed()) {
                observer.update(game, true, true);
                observer.renderDialog("[Поражение...]\n[Начинается новая игра...]\n", true, 3);
                game.endGame(false);
                isGameOver = false;
                if (preparePhase() == false) run();
                round();
                break;
            } else if (game.getGameState().getEnemyShipManager().allShipsDestroyed()) {
                observer.update(game, false, true);
                observer.renderDialog("[ПОБЕДА!!!]\n[Начинается новый раунд...]\n", true, 3);
                game.endGame(true);
                isGameOver = false;
                preparePhase(true);
                round();
                break;
            }
        }

    }

    std::pair<std::string, bool> getFileName() {
        Command command;
        bool canceled = false;
        command = commandHandler.getCommand();
        switch (command) {
            case Command::CANCEL:
                canceled = true;
                break;
        }
    }

    bool confirm() {
        Command command;
        bool confirm = false;
        while (!confirm) {
            command = commandHandler.getCommand();
            switch (command) {
                case Command::YES:
                    confirm = true;
                    return true;
                case Command::NO:
                    confirm = true;
                    return false;
                default:
                    break;
            }
        }
    }

    void cancelInfo() {
        Command command;
        bool cancel = false;
        while (!cancel) {
            command = commandHandler.getCommand();
            switch (command) {
                case Command::CANCEL:
                    cancel = true;
                    return;
                default:
                    break;
            }
        }
    }
    
    std::vector<Length> getShipSizesFromUser() {
        int numShips = selectInt("Введите количество кораблей: ", 1, 100);

        std::vector<Length> shipSizes;
        for (int i = 0; i < numShips; ++i) {
            int size = selectInt("Введите длину корабля [" + std::to_string(i+1) + "]: ", 1, 4);
            shipSizes.push_back(static_cast<Length>(size));
        }

        return shipSizes;
    }

    std::pair<int, int> getFieldDimensionsFromUser() {
        int height = selectInt("Введите высоту поля: ", 1, 1000);
        int width = selectInt("Введите ширину поля: ", 1, 1000);

        return std::make_pair(height, width);
    }

    std::pair<int, int> selectCell(const std::string& actionDescription, bool isEnemy, int XLine = 1, int YLine = 1) {
        int tempCursorX = 0, tempCursorY = 0;
        Command command;
        bool select = false;

        while (!select) {
            observer.update(game, isEnemy, true, tempCursorX, tempCursorY, XLine, YLine);

            observer.renderDialog(actionDescription + "\n[" + map[Command::MOVE_UP] + "][" + map[Command::MOVE_LEFT] + "][" + map[Command::MOVE_DOWN] + "][" + map[Command::MOVE_RIGHT] + "] - перемещение, [" + map[Command::ENTER] + "] - выбор, [" + map[Command::CANCEL] + "] - отмена\n");

            command = commandHandler.getCommand();

            switch (command) {
                case Command::MOVE_UP: if (tempCursorY > 0) --tempCursorY; break;
                case Command::MOVE_DOWN: if (tempCursorY < game.getGameState().getHeight() - 1) ++tempCursorY; break;
                case Command::MOVE_LEFT: if (tempCursorX > 0) --tempCursorX; break;
                case Command::MOVE_RIGHT: if (tempCursorX < game.getGameState().getWidth() - 1) ++tempCursorX; break;
                case Command::ENTER:
                    select = true;
                    return {tempCursorX, tempCursorY};
                case Command::CANCEL:
                    select = true;
                    return {-1, -1};
                default:
                    break;
            }
        }
    }

    std::vector<int> selectCellForShip(const std::string& actionDescription, Length len, bool isEnemy) {
        int tempCursorX = 0, tempCursorY = 0;
        int XLine = 1, YLine = 1;
        bool Horizontal = true;
        Command command;
        bool select = false;

        while (!select) {
            if (Horizontal) {
                XLine = static_cast<int>(len);
                YLine = 1;
            } else {
                XLine = 1;
                YLine = static_cast<int>(len);           
            }
            observer.update(game, isEnemy, true, tempCursorX, tempCursorY, XLine, YLine);
            observer.renderDialog(actionDescription + "\n[" + map[Command::MOVE_UP] + "][" + map[Command::MOVE_LEFT] + "][" + map[Command::MOVE_DOWN] + "][" + map[Command::MOVE_RIGHT] + "] - перемещение, [" + map[Command::ORIENTATION] + "] - поворот, [" + map[Command::ENTER] + "] - выбор, [" + map[Command::CANCEL] + "] - в главное меню\n");
            command = commandHandler.getCommand();

            switch (command) {
                case Command::MOVE_UP: if (tempCursorY > 0) --tempCursorY; break;
                case Command::MOVE_DOWN:
                    if (Horizontal) {
                        if (tempCursorY < game.getGameState().getHeight() - 1) ++tempCursorY; break;
                    } else {
                        if (tempCursorY < game.getGameState().getHeight() - YLine) ++tempCursorY; break;
                    }
                case Command::MOVE_LEFT: if (tempCursorX > 0) --tempCursorX; break;
                case Command::MOVE_RIGHT:
                    if (Horizontal) {
                        if (tempCursorX < game.getGameState().getWidth() - XLine) ++tempCursorX; break;
                    } else {
                        if (tempCursorX < game.getGameState().getWidth() - 1) ++tempCursorX; break;
                    }
                case Command::ORIENTATION:
                    if (Horizontal == true && tempCursorY > game.getGameState().getHeight() - XLine) break;
                    if (Horizontal == false && tempCursorX > game.getGameState().getWidth() - YLine) break;
                    Horizontal = !Horizontal;
                    break;
                case Command::ENTER:
                    select = true;
                    return {tempCursorX, tempCursorY, static_cast<int>(!Horizontal)};
                case Command::CANCEL:
                    select = true;
                    return {-1,-1,-1};
                default:
                    break;
            }
        }
    }
    int selectInt(const std::string& actionDescription, int minValue, int maxValue) {
        
        int selectedValue = minValue;
        bool select = false;

        while (!select) {
            observer.update(game, false, false);
            observer.renderDialog(actionDescription + getDisplayValueStr(selectedValue) + "\n[" + map[Command::MOVE_LEFT] + "] - меньше, [" + map[Command::MOVE_RIGHT] + "] - больше, [" + map[Command::ENTER] + "] - выбор\n");
            Command command = commandHandler.getCommand();

            switch (command) {
                case Command::MOVE_LEFT:
                    if (selectedValue > minValue) --selectedValue;
                    break;
                case Command::MOVE_RIGHT:
                    if (selectedValue < maxValue) ++selectedValue;
                    break;
                case Command::ENTER:
                    select = true;
                    return selectedValue;
                default:
                    break;
            }
        }
    }

private:
    std::string getDisplayValueStr(int value) const {
        return "\n\033[2K\033[1G<- [ " + std::to_string(value) + " ] ->" + '\n';
    }

    Game& game;
    CommandHandler& commandHandler;
    GameObserver<Renderer>& observer;
    bool isEnemyField = false;
    bool isGameOver = false;
    std::unordered_map<Command, std::string> map;
};

#endif GAME_CONTROLLER_H