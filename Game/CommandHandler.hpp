#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include <iostream>
#include <unordered_map>
#include <string>
#include "json.hpp"
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include "GameObserver.hpp"
#include "../Exceptions/ConfigFileException.hpp"
#include "../Exceptions/NoKeyException.hpp"
#include "../Exceptions/JSONexception.hpp"
#include "../Exceptions/LoadFileException.hpp"
#include "../Exceptions/SecondCommandException.hpp"
#include "../Exceptions/SecondKeyException.hpp"
#include "../Exceptions/UnknownCommandException.hpp"

enum class Command {
    //select
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ENTER,
    //menu
    PLAY,
    SAVE,
    LOAD,
    EXIT,
    CONTINUE,
    //place
    PLACE,
    ORIENTATION,
    //always
    FIELD_SWITCH,
    CANCEL, 
    //turn
    END_TURN,
    ATTACK,
    USE_ABILITY,
    SHOW_ABILITIES,
    MENU,
    //other
    YES,
    NO,
};

class CommandHandler {
public:
    CommandHandler(const std::string& filename);
    Command getCommand();
    std::string getFileName();
    void loadDefaultMapping();
    void loadMappingFromFile(const std::string& filename);
    void validateMapping();
    void loadConfig();
    std::unordered_map<Command, std::string>& getReverseKeyMapping();

private:
    std::unordered_map<char, Command> keyMapping;
    std::unordered_map<Command, std::string> reverseKeyMapping;
    std::string config;
};

#endif COMMAND_HANDLER_H