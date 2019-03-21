#include <cstring>
#include <cstdio>

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include "utf8string.hpp"


Utf8String::Utf8String() {}

Utf8String::Utf8String(std::vector<value_type> &&temp) noexcept : data(std::move(temp))
{
    for (auto chr : data)
    {
        if (chr < 0b10000000U) raw_string += chr;
        else if (chr < 0b111U << 13) (raw_string += chr >> 8 & 0xFF) += chr & 0xFF;
        else if (chr < 0b1111U << 20) ((raw_string += chr >> 16 & 0xFF) += chr >> 8 & 0xFF) += chr & 0xFF;
        else if (chr < 0b11111U << 27) (((raw_string += chr >> 24 & 0xFF) += chr >> 16 & 0xFF) += chr >> 8 & 0xFF) += chr & 0xFF;
        else break;
    }
}

Utf8String::Utf8String(const raw_type &raw_string) : raw_string(raw_string)
{
    value_type temp;

    auto reading = (unsigned char*)raw_string.c_str();
    while (*reading != 0)
    {
        if (*reading < 0b10000000U) data.push_back(*reading++);
        else if (*reading < 0b11100000U)
        {
            (temp = *reading++) <<= 8;
            data.push_back(temp |= *reading++);
        }
        else if (*reading < 0b11110000U)
        {
            (temp = *reading++) <<= 8;
            (temp |= *reading++) <<= 8;
            data.push_back(temp |= *reading++);
        }
        else if (*reading < 0b11111000U)
        {
            (temp = *reading++) <<= 8;
            (temp |= *reading++) <<= 8;
            (temp |= *reading++) <<= 8;
            data.push_back(temp |= *reading++);
        }
        else break;
        temp = 0;
    }
}

Utf8String::Utf8String(const char *raw_string) : Utf8String(std::string(raw_string)) {}

Utf8String::Utf8String(const Utf8String &rhs) : data(rhs.data), raw_string(rhs.raw_string) {}

Utf8String::Utf8String(Utf8String &&rhs) noexcept : data(std::move(rhs.data)), raw_string(std::move(rhs.raw_string)) {}

Utf8String::~Utf8String() {}


Utf8String& Utf8String::operator=(const raw_type &raw_string)
{
    data.clear();
    this->raw_string = raw_string;

    value_type temp;

    auto reading = (unsigned char*)raw_string.c_str();
    while (*reading != 0)
    {
        if (*reading < 0b10000000U) data.push_back(*reading++);
        else if (*reading < 0b11100000U)
        {
            (temp = *reading++) <<= 8;
            data.push_back(temp |= *reading++);
        }
        else if (*reading < 0b11110000U)
        {
            (temp = *reading++) <<= 8;
            (temp |= *reading++) <<= 8;
            data.push_back(temp |= *reading++);
        }
        else if (*reading < 0b11111000U)
        {
            (temp = *reading++) <<= 8;
            (temp |= *reading++) <<= 8;
            (temp |= *reading++) <<= 8;
            data.push_back(temp |= *reading++);
        }
        else break;
        temp = 0;
    }

    return *this;
}

Utf8String& Utf8String::operator=(const char *raw_string)
{
    return operator=(std::string(raw_string));
}

Utf8String& Utf8String::operator=(const Utf8String &rhs)
{
    data = rhs.data;
    raw_string = rhs.raw_string;
    return *this;
}

Utf8String& Utf8String::operator=(Utf8String &&rhs) noexcept
{
    std::swap(data, rhs.data);
    std::swap(raw_string, rhs.raw_string);
    return *this;
}

Utf8String::const_reference Utf8String::operator[](std::size_t index) const
{
    return data[index];
}


Utf8String Utf8String::substr(size_type pos, size_type count) const
{
    if (count == npos) return std::vector<value_type>(data.begin() + pos, data.end());
    return std::vector<value_type>(data.begin() + pos, data.begin() + std::min(size(), pos + count));
}

Utf8String::size_type Utf8String::size() const noexcept
{
    return data.size();
}

Utf8String::raw_type Utf8String::raw() const noexcept
{
    return raw_string;
}

const char* Utf8String::c_str() const noexcept
{
    return raw_string.c_str();
}


bool operator==(const Utf8String &lhs, const Utf8String &rhs)
{
    return lhs.raw_string == rhs.raw_string;
}

bool operator!=(const Utf8String &lhs, const Utf8String &rhs)
{
    return lhs.raw_string != rhs.raw_string;
}

bool operator<(const Utf8String &lhs, const Utf8String &rhs)
{
    return lhs.raw_string < rhs.raw_string;
}
