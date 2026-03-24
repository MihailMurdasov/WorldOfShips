#ifndef JSON_EXCEPTION_H
#define JSON_EXCEPTION_H

#include <exception>

class JSONException : public std::exception {
public:
    const char* what() const noexcept {
        return "[ ERROR ]: Файл должен иметь расширение .json\n";
    }
};

#endif JSON_EXCEPTION_H