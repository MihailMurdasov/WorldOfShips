#ifndef SAVE_FILE_EXCEPTION_H
#define SAVE_FILE_EXCEPTION_H

#include <exception>

class SaveFileException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Не удалось сохранить игру.\n";
    }
};

#endif SAVE_FILE_EXCEPTION_H