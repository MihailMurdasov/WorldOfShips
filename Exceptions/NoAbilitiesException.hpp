#ifndef NO_ABILITIES_EXCEPTION_H
#define NO_ABILITIES_EXCEPTION_H

#include <exception>

class NoAbilitiesException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Нет доступных способностей.\n";
    }
};

#endif NO_ABILITIES_EXCEPTION_H