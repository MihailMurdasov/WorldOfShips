#ifndef NO_KEY_EXCEPTION_H
#define NO_KEY_EXCEPTION_H

#include <exception>

class NoKeyException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ CONFIG ERROR ]: Команда не имеет назначенной клавиши.\n";
    }
};

#endif NO_KEY_EXCEPTION_H