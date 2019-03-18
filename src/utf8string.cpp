#include <dse.hpp>


Utf8String::Utf8String(const raw_type &raw_string) : raw_string(raw_string)
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

Utf8String::Utf8String(const Utf8String &rhs) : raw_string(rhs.raw_string), data(rhs.data)
{
    // nothing
}

Utf8String::Utf8String(Utf8String &&rhs) noexcept
{
    std::swap
}

Utf8String::~Utf8String()
{
    // nothing
}

const Utf8String::size_type Utf8String::size() const noexcept
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
