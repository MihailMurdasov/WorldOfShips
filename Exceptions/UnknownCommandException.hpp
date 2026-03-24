#ifndef UNKNOWN_COMMAND_EXCEPTION_H
#define UNKNOWN_COMMAND_EXCEPTION_H

#include <exception>

class UnknownCommandException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ CONFIG ERROR ]: Неизвестная команда.\n";
    }
};

#endif UNKNOWN_COMMAND_EXCEPTION_H