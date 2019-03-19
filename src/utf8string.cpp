#include <cstring>

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <functional>

#include "utf8string.hpp"


using U8S = Utf8String;


U8S::Utf8String(std::vector<value_type> &&temp) noexcept : data(std::move(temp))
{
    for (auto chr: data)
    {
        switch (chr & 0b11111000111100001110000000000000)
        {
        case 0:
            raw_string += *(((char*)&chr)+3);
            break;
        case 0b11 << 14:
            raw_string += *(((char*)&chr)+2);
            raw_string += *(((char*)&chr)+3);
            break;
        case 0b111000001 << 15:
            raw_string += *(((char*)&chr)+1);
            raw_string += *(((char*)&chr)+2);
            raw_string += *(((char*)&chr)+3);
            break;
        case 0b11110000100000001 << 15:
            raw_string += *(((char*)&chr)+0);
            raw_string += *(((char*)&chr)+1);
            raw_string += *(((char*)&chr)+2);
            raw_string += *(((char*)&chr)+3);
        default:
            break;
        }
    }
}

U8S::Utf8String(const raw_type &raw_string) : raw_string(raw_string)
{
    value_type temp;

    auto reading = raw_string.c_str();
    while (*reading != '\0')
    {
        switch (*reading & 0b11111000)
        {
        case 0b00000000:
            data.push_back(*reading++);
            break;
        case 0b11000000:
            data.push_back((((temp = *reading++) <<= 8)) |= *reading++);
            break;
        case 0b11100000:
            data.push_back((((((temp = *reading++) <<= 8)) |= *reading++) <<= 8) |= *reading++);
            break;
        case 0b11110000:
            data.push_back((((((((temp = *reading++) <<= 8)) |= *reading++) <<= 8) |= *reading++) <<= 8) |= *reading++);
            break;
        default:
            break; // TODO: try to fix bugs when input is not utf-8
        }

        temp = 0;
    }
}

U8S::Utf8String(const U8S &rhs) : data(rhs.data), raw_string(rhs.raw_string)
{
    // nothing
}

U8S::Utf8String(U8S &&rhs) noexcept : data(std::move(rhs.data)), raw_string(std::move(rhs.raw_string))
{
    // nothing
}

U8S::~Utf8String()
{
    // nothing
}

U8S& U8S::operator=(const raw_type &raw_string)
{
    this->raw_string = raw_string;

    value_type temp;

    auto reading = raw_string.c_str();
    while (*reading != '\0')
    {
        switch (*reading & 0b11111000)
        {
        case 0:
            data.push_back(*reading++);
            break;
        case 0b11000000:
            data.push_back((((temp = *reading++) <<= 8)) |= *reading++);
            break;
        case 0b11100000:
            data.push_back((((((temp = *reading++) <<= 8)) |= *reading++) <<= 8) |= *reading++);
            break;
        case 0b11110000:
            data.push_back((((((((temp = *reading++) <<= 8)) |= *reading++) <<= 8) |= *reading++) <<= 8) |= *reading++);
            break;
        default:
            break; // TODO: try to fix bugs when input is not utf-8
        }

        temp = 0;
    }

    return *this;
}

U8S& U8S::operator=(const U8S &rhs)
{
    data = rhs.data;
    raw_string = rhs.raw_string;
}

U8S& U8S::operator=(U8S &&rhs) noexcept
{
    std::swap(data, rhs.data);
    std::swap(raw_string, rhs.raw_string);
}

U8S::reference U8S::operator[](std::size_t index)
{
    return data[index];
}

U8S::const_reference U8S::operator[](std::size_t index) const
{
    return data[index];
}

Utf8String U8S::substr(size_type pos, size_type count) const
{
    return std::vector<value_type>(data.begin() + pos, data.begin() + std::min(size(), pos + count));
}

const U8S::size_type U8S::size() const noexcept
{
    return data.size();
}

U8S::raw_type U8S::raw() const noexcept
{
    return raw_string;
}

const char* U8S::c_str() const noexcept
{
    return raw_string.c_str();
}

bool operator==(const U8S &lhs, const Utf8String &rhs)
{
    return lhs.raw_string == rhs.raw_string;
}

bool operator<(const Utf8String &lhs, const Utf8String &rhs)
{
    return lhs.raw_string < rhs.raw_string;
}
