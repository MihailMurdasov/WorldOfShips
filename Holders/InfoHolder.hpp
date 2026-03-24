#ifndef INFO_HOLDER_H
#define INFO_HOLDER_H

#include <variant>
#include <string>

class InfoHolder {
public:
    virtual void need(std::string name) = 0;
    virtual std::variant<int, std::string, bool> get(std::string name) = 0;
    ~InfoHolder() = default;
};

#endif INFO_HOLDER_H