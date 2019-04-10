#include <algorithm>

#include "utf8string.hpp"


Utf8String::Utf8String() {}

Utf8String::Utf8String(data_type&& data) noexcept : data_(std::move(data)) {}

Utf8String::Utf8String(const raw_type& raw_string_) : raw_string_(raw_string_)
{
	value_type temp;

	auto reading = (unsigned char*)raw_string_.c_str();
	while (*reading != 0)
	{
		if (*reading < 0b10000000U) data_.push_back(*reading++);
		else if (*reading < 0b11100000U)
		{
			(temp = *reading++) <<= 8;
			data_ += (temp |= *reading++);
		}
		else if (*reading < 0b11110000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data_ += (temp |= *reading++);
		}
		else if (*reading < 0b11111000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data_ += (temp |= *reading++);
		}
		else break;
		temp = 0;
	}
}

Utf8String::Utf8String(const char* raw_string) : Utf8String(std::string(raw_string)) {}

Utf8String::Utf8String(value_type ch) : Utf8String(1, ch) {}

Utf8String::Utf8String(size_type count, value_type ch) : Utf8String(data_type(count, ch)) {}

Utf8String::Utf8String(const Utf8String & rhs) : data_(rhs.data_), raw_string_(rhs.raw_string_) {}

Utf8String::Utf8String(Utf8String && rhs) noexcept : data_(std::move(rhs.data_)), raw_string_(std::move(rhs.raw_string_)) {}

Utf8String::~Utf8String() {}


Utf8String& Utf8String::operator=(const raw_type & raw_string)
{
	data_.clear();
	raw_string_ = raw_string;

	value_type temp;

	auto reading = (unsigned char*)raw_string_.c_str();
	while (*reading != 0)
	{
		if (*reading < 0b10000000U) data_ += *reading++;
		else if (*reading < 0b11100000U)
		{
			(temp = *reading++) <<= 8;
			data_ += (temp |= *reading++);
		}
		else if (*reading < 0b11110000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data_ += (temp |= *reading++);
		}
		else if (*reading < 0b11111000U)
		{
			(temp = *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			(temp |= *reading++) <<= 8;
			data_ += (temp |= *reading++);
		}
		else break;
		temp = 0;
	}

	return *this;
}

Utf8String& Utf8String::operator=(const char* raw_string)
{
	return operator=(std::string(raw_string));
}

Utf8String& Utf8String::operator=(const Utf8String & rhs)
{
	data_ = rhs.data_;
	raw_string_ = rhs.raw_string_;
	return *this;
}

Utf8String& Utf8String::operator=(Utf8String && rhs) noexcept
{
	std::swap(data_, rhs.data_);
	std::swap(raw_string_, rhs.raw_string_);
	return *this;
}

Utf8String::reference Utf8String::operator[](size_type index)
{
	raw_string_.clear();
	return data_[index];
}

Utf8String::const_reference Utf8String::operator[](size_type index) const
{
	return data_[index];
}

Utf8String& Utf8String::operator+=(value_type rhs)
{
	data_ += rhs;
	raw_string_.clear();
	return *this;
}

Utf8String& Utf8String::operator+=(const Utf8String & rhs)
{
	data_ += rhs.data_;
	raw_string_ += rhs.raw();
	return *this;
}


Utf8String::value_type& Utf8String::front()
{
	raw_string_.clear();
	return data_.front();
}

const Utf8String::value_type& Utf8String::front() const
{
	return data_.front();
}

Utf8String::value_type& Utf8String::back()
{
	raw_string_.clear();
	return data_.back();
}

const Utf8String::value_type& Utf8String::back() const
{
	return data_.back();
}

void Utf8String::clear()
{
	data_.clear();
	raw_string_.clear();
}

void Utf8String::push_back(value_type ch)
{
	data_.push_back(ch);
	raw_string_.clear();
}

Utf8String Utf8String::substr(size_type pos, size_type count) const
{
	return data_.substr(pos, count);
}

Utf8String::size_type Utf8String::size() const noexcept
{
	return data_.size();
}

Utf8String::size_type Utf8String::length() const noexcept
{
	return data_.length();
}

bool Utf8String::empty() const noexcept
{
	return data_.empty();
}

Utf8String::raw_type Utf8String::raw()
{
	if (!raw_string_.empty()) return raw_string_;

	for (auto chr : data_)
	{
		if (chr < 0b10000000U) raw_string_ += chr;
		else if (chr < 0b111U << 13) (raw_string_ += chr >> 8 & 0xFF) += chr & 0xFF;
		else if (chr < 0b1111U << 20) ((raw_string_ += chr >> 16 & 0xFF) += chr >> 8 & 0xFF) += chr & 0xFF;
		else if (chr < 0b11111U << 27) (((raw_string_ += chr >> 24 & 0xFF) += chr >> 16 & 0xFF) += chr >> 8 & 0xFF) += chr & 0xFF;
		else break;
	}

	return raw_string_;
}

Utf8String::raw_type Utf8String::raw() const noexcept
{
	return raw_string_;
}

const char* Utf8String::c_str()
{
	return raw().c_str();
}

const char* Utf8String::c_str() const noexcept
{
	return raw_string_.c_str();
}


Utf8String::size_type Utf8String::find(const Utf8String & str, size_type pos) const
{
	return data_.find(str.data_, pos);
}


Utf8String operator+(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data_ + rhs.data_;
}

bool operator==(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data_ == rhs.data_;
}

bool operator!=(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data_ != rhs.data_;
}

bool operator<(const Utf8String & lhs, const Utf8String & rhs)
{
	return lhs.data_ < rhs.data_;
}

std::ostream& operator<<(std::ostream & out, const Utf8String & str)
{
	return out << str.raw();
}

std::istream& operator>>(std::istream & in, Utf8String & str)
{
	in >> str.raw_string_;
	str = str.raw_string_;
	return in;
}
