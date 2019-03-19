#include <dse.hpp>


using U8S = Utf8String;


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

U8S::Utf8String(U8S &&rhs) noexcept
{
    std::swap(data, rhs.data);
    std::swap(raw_string, rhs.raw_string);
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
