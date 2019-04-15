#include "commucation.hpp"


CommProtocol::CommProtocol(const std::string &raw) : type_(static_cast<TYPE>(raw[0] - '0')), content_(raw.substr(1))
{
    // ...
}

CommProtocol::CommProtocol(TYPE type, const std::string &content) : type_(type), content_(content)
{
    // ...
}

CommProtocol::TYPE CommProtocol::type() const
{
    return type_;
}

std::string CommProtocol::content() const
{
    return content_;
}

std::string CommProtocol::to_string() const
{
    return std::to_string(type_) + content_;
}

std::string CommProtocol::to_string(const CommProtocol &commp)
{
    return commp.to_string();
}

std::string CommProtocol::to_string(TYPE type, const std::string &content)
{
    return CommProtocol(type, content).to_string();
}
