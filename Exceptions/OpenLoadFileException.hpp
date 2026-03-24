#ifndef LOAD_OPEN_FILE_EXCEPTION_H
#define LOAD_OPEN_FILE_EXCEPTION_H

#include <exception>

class OpenLoadFileException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Не удалось открыть файл загрузки. Проверьте путь к файлу.\n";
    }
};

#endif LOAD_OPEN_FILE_EXCEPTION_H