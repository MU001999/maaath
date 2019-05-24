#pragma once

#include <string>

class Utf8String
{
public:
	using data_type = std::u32string;
	using value_type = data_type::value_type;
	using raw_type = std::string;
	using size_type = data_type::size_type;
	using reference = value_type &;
	using const_reference = const value_type &;
	using pointer = value_type *;
	using const_pointer = const value_type &;

private:
	data_type data_;
	raw_type raw_string_;

public:
	// this is a special value equal to the maximum value representable by the type size_type. The exact meaning depends on context, but it is generally used either as end of string indicator by the functions that expect a string index or as the error indicator by the functions that return a string index.
	static const size_type npos = -1;

	// default constructor
	Utf8String();

	// constructor, receives std::vector<value_type>
	Utf8String(data_type&& data) noexcept;

	// constructor, receives raw_string
	Utf8String(const raw_type& raw_string);

	// constructor, receives raw_string
	Utf8String(const char* raw_string);

	// constructor, receives single character
	Utf8String(value_type ch);

	// constructor, receives single char
	Utf8String(size_type count, value_type ch);

	// copy constructor
	Utf8String(const Utf8String& rhs);

	// move constructor
	Utf8String(Utf8String&& rhs) noexcept;

	// destructor, destroys the string, deallocating internal storage if used
	~Utf8String();

	// assigns values to the string
	Utf8String& operator=(const raw_type& raw_string);

	// assigns values to the string
	Utf8String& operator=(const char* raw_string);

	// assigns values to the string
	Utf8String& operator=(const Utf8String& rhs);

	// assigns values to the string
	Utf8String& operator=(Utf8String&& rhs) noexcept;

	// accesses the specified character
	reference operator[](size_type index);

	// accesses the specified character
	const_reference operator[](size_type index) const;

	// appends characters to the end
	Utf8String& operator+=(value_type rhs);

	// appends characters to the end
	Utf8String& operator+=(const Utf8String& rhs);

	// accesses the first character
	value_type& front();
	
	// accesses the first character
	const value_type& front() const;

	// accesses the last character 
	value_type& back();

	// accesses the last character 
	const value_type& back() const;

	// clears the contents 
	void clear();

	// appends a character to the end
	void push_back(value_type ch);

	// returns a substring [pos, pos+count). if the requested substring extends past the end of the string, or if count == npos, the returned substring is [pos, size()).
	Utf8String substr(size_type pos, size_type count = npos) const;

	// returns the number of characters
	size_type size() const noexcept;

	// returns the number of characters
	size_type length() const noexcept;

	// checks whether the string is empty 
	bool empty() const noexcept;

	// returns the raw string
	raw_type raw();

	// returns the raw string
	raw_type raw() const noexcept;

	// returns a non-modifiable standard C character array version of the string
	const char* c_str();

	// returns a non-modifiable standard C character array version of the string
	const char* c_str() const noexcept;

	// find characters in the string 
	size_type find(const Utf8String& str, size_type pos = 0) const;
	 
	friend Utf8String operator+(const Utf8String& lhs, const Utf8String& rhs);

	friend bool operator==(const Utf8String& lhs, const Utf8String& rhs);

	friend bool operator!=(const Utf8String& lhs, const Utf8String& rhs);
	 
	friend bool operator<(const Utf8String& lhs, const Utf8String& rhs);

	friend std::ostream& operator<<(std::ostream& out, const Utf8String& str);

	friend std::istream& operator>>(std::istream& in, Utf8String& str);
};

// concatenates two strings or a string and a char 
extern Utf8String operator+(const Utf8String& lhs, const Utf8String& rhs);

// lexicographically compares two strings 
extern bool operator==(const Utf8String& lhs, const Utf8String& rhs);

// lexicographically compares two strings 
extern bool operator!=(const Utf8String& lhs, const Utf8String& rhs);

// lexicographically compares two strings
extern bool operator<(const Utf8String& lhs, const Utf8String& rhs);

// performs stream input and output on strings 
extern std::ostream& operator<<(std::ostream& out, const Utf8String& str);

// performs stream input and output on strings 
extern std::istream& operator>>(std::istream& in, Utf8String& str);