#ifndef CONFIG_FILE_EXCEPTION_H
#define CONFIG_FILE_EXCEPTION_H

#include <exception>

class ConfigFileException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ CONFIG ERROR ]: Не удалось открыть файл для загрузки конфигурации.\n";
    }
};

#endif CONFIG_FILE_EXCEPTION_H