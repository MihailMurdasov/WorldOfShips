#ifndef OPEN_SAVE_FILE_EXCEPTION_H
#define OPEN_SAVE_FILE_EXCEPTION_H

#include <exception>

class OpenSaveFileException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Не удалось открыть файл для сохранения.\n";
    }
};

#endif OPEN_SAVE_FILE_EXCEPTION_H