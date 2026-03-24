#ifndef SECOND_KEY_EXCEPTION_H
#define SECOND_KEY_EXCEPTION_H

#include <exception>

class SecondKeyException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ CONFIG ERROR ]: Одна клавиша назначена на более, чем одну команду.\n";
    }
};

#endif SECOND_KEY_EXCEPTION_H