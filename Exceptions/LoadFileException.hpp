#ifndef LOAD_FILE_EXCEPTION_H
#define LOAD_FILE_EXCEPTION_H

#include <exception>

class LoadFileException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Не удалось загрузить игру из файла.\n";
    }
};

#endif LOAD_FILE_EXCEPTION_H