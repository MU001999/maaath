#include "commucation.hpp"


Request::Request(const std::string &raw) : type_(static_cast<CommType>(raw[0] - '0')), keywords_(raw.substr(1))
{
    // ...
}

Request::CommType Request::type() const
{
    return type_;
}

std::string Request::keywords() const
{
    return keywords_;
}
