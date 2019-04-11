#pragma once

#include <string>


class CommProtocol
{
public:
    enum TYPE
    {
        Article = 0,
        Definition,
        Property,
        Example,
        Exercise
    };

    CommProtocol(const std::string &raw);
    CommProtocol(TYPE type, const std::string &content);
    ~CommProtocol() = default;

    std::string to_string() const;

    static std::string to_string(const CommProtocol &commp);
    static std::string to_string(TYPE type, const std::string &content);

private:
    TYPE type_;
    std::string content_;
};
