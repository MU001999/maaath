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

Utf8String::Utf8String(data_type&& data) noexcept : data(std::move(data)) {}

Utf8String::Utf8String(const raw_type& raw_string) : raw_string(raw_string)
{
	value_type temp;

	auto reading = (unsigned char*)raw_string.c_str();
	while (*reading != 0)
	{
		if (*reading < 0b10000000U) data.push_back(*reading++);
		else if (*reading < 0b11100000U)
		{
			(temp = *reading++) <<= 8;
			data += (temp |= *reading++);
		}
		else if (*reading < 0b11110000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data += (temp |= *reading++);
		}
		else if (*reading < 0b11111000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data += (temp |= *reading++);
		}
		else break;
		temp = 0;
	}
}

Utf8String::Utf8String(const char* raw_string) : Utf8String(std::string(raw_string)) {}

Utf8String::Utf8String(value_type ch) : Utf8String(1, ch) {}

Utf8String::Utf8String(size_type count, value_type ch) : Utf8String(data_type(count, ch)) {}

Utf8String::Utf8String(const Utf8String & rhs) : data(rhs.data), raw_string(rhs.raw_string) {}

Utf8String::Utf8String(Utf8String && rhs) noexcept : data(std::move(rhs.data)), raw_string(std::move(rhs.raw_string)) {}

Utf8String::~Utf8String() {}


Utf8String& Utf8String::operator=(const raw_type & raw_string)
{
	data.clear();
	this->raw_string = raw_string;

	value_type temp;

	auto reading = (unsigned char*)raw_string.c_str();
	while (*reading != 0)
	{
		if (*reading < 0b10000000U) data += *reading++;
		else if (*reading < 0b11100000U)
		{
			(temp = *reading++) <<= 8;
			data += (temp |= *reading++);
		}
		else if (*reading < 0b11110000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data += (temp |= *reading++);
		}
		else if (*reading < 0b11111000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data += (temp |= *reading++);
		}
		else break;
		temp = 0;
	}

	return *this;
}

Utf8String & Utf8String::operator=(const char* raw_string)
{
	return operator=(std::string(raw_string));
}

Utf8String& Utf8String::operator=(const Utf8String & rhs)
{
	data = rhs.data;
	raw_string = rhs.raw_string;
	return *this;
}

Utf8String& Utf8String::operator=(Utf8String && rhs) noexcept
{
	std::swap(data, rhs.data);
	std::swap(raw_string, rhs.raw_string);
	return *this;
}

Utf8String::reference Utf8String::operator[](size_type index)
{
	raw_string.clear();
	return data[index];
}

Utf8String::const_reference Utf8String::operator[](size_type index) const
{
	return data[index];
}

Utf8String& Utf8String::operator+=(value_type rhs)
{
	data += rhs;
	raw_string.clear();
	return *this;
}

Utf8String& Utf8String::operator+=(const Utf8String & rhs)
{
	data += rhs.data;
	raw_string += rhs.raw();
	return *this;
}


Utf8String::value_type& Utf8String::front()
{
	raw_string.clear();
	return data.front();
}

const Utf8String::value_type& Utf8String::front() const
{
	return data.front();
}

Utf8String::value_type& Utf8String::back()
{
	raw_string.clear();
	return data.back();
}

const Utf8String::value_type& Utf8String::back() const
{
	return data.back();
}

void Utf8String::clear()
{
	data.clear();
	raw_string.clear();
}

void Utf8String::push_back(value_type ch)
{
	data.push_back(ch);
	raw_string.clear();
}

Utf8String Utf8String::substr(size_type pos, size_type count) const
{
	return data.substr(pos, count);
}

Utf8String::size_type Utf8String::size() const noexcept
{
	return data.size();
}

Utf8String::size_type Utf8String::length() const noexcept
{
	return data.length();
}

bool Utf8String::empty() const noexcept
{
	return data.empty();
}

Utf8String::raw_type Utf8String::raw()
{
	if (!raw_string.empty()) return raw_string;

	for (auto chr : data)
	{
		if (chr < 0b10000000U) raw_string += chr;
		else if (chr < 0b111U << 13) (raw_string += chr >> 8 & 0xFF) += chr & 0xFF;
		else if (chr < 0b1111U << 20) ((raw_string += chr >> 16 & 0xFF) += chr >> 8 & 0xFF) += chr & 0xFF;
		else if (chr < 0b11111U << 27) (((raw_string += chr >> 24 & 0xFF) += chr >> 16 & 0xFF) += chr >> 8 & 0xFF) += chr & 0xFF;
		else break;
	}

	return raw_string;
}

Utf8String::raw_type Utf8String::raw() const noexcept
{
	return raw_string;
}

const char* Utf8String::c_str()
{
	return raw().c_str();
}

const char* Utf8String::c_str() const noexcept
{
	return raw_string.c_str();
}


Utf8String::size_type Utf8String::find(const Utf8String & str, size_type pos) const
{
	return data.find(str.data, pos);
}


Utf8String operator+(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data + rhs.data;
}

bool operator==(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data == rhs.data;
}

bool operator!=(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data != rhs.data;
}

bool operator<(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data < rhs.data;
}

std::ostream& operator<<(std::ostream & out, const Utf8String & str)
{
	return out << str.raw();
}

std::istream& operator>>(std::istream & in, Utf8String & str)
{
	in >> str.raw_string;
	str = str.raw_string;
	return in;
}
