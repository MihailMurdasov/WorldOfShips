#ifndef SECOND_COMMAND_EXCEPTION_H
#define SECOND_COMMAND_EXCEPTION_H

#include <exception>

class SecondCommandException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ CONFIG ERROR ]: Одна команда назначена на более, чем одну клавишу.\n";
    }
};

#endif SECOND_COMMAND_EXCEPTION_H