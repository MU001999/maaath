#pragma once


class Utf8String
{
public:
    using data_type = std::u32string;
    using value_type = data_type::value_type;
    using raw_type = std::string;
    using size_type = data_type::size_type;
    using reference = value_type & ;
    using const_reference = const value_type&;
    using pointer = value_type * ;
    using const_pointer = const value_type&;

private:
    data_type data;
    raw_type raw_string;

public:
    // This is a special value equal to the maximum value representable by the type size_type. The exact meaning depends on context, but it is generally used either as end of string indicator by the functions that expect a string index or as the error indicator by the functions that return a string index.
    static const size_type npos = -1;


    Utf8String();

    // Constructor, receives std::vector<value_type>
    Utf8String(data_type &&data) noexcept;

    // Constructor, receives raw_string
    Utf8String(const raw_type &raw_string);

    // Constructor, receives raw_string
    Utf8String(const char *raw_string);

    Utf8String(value_type ch);

    // Consturctor, receives single char
    Utf8String(size_type count, value_type ch);

    // Copy constructor
    Utf8String(const Utf8String &rhs);

    // Move constructor
    Utf8String(Utf8String &&rhs) noexcept;

    // Destructor, destroys the string, deallocating internal storage if used
    ~Utf8String();


    Utf8String& operator=(const raw_type &raw_string);

    Utf8String& operator=(const char *raw_string);

    Utf8String& operator=(const Utf8String &rhs);

    Utf8String& operator=(Utf8String &&rhs) noexcept;

    // Accesses the specified character
    reference operator[](size_type index);

    const_reference operator[](size_type index) const;

    Utf8String& operator+=(value_type rhs);

    Utf8String& operator+=(const Utf8String &rhs);


    value_type& front();

    const value_type& front() const;

    value_type& back();

    const value_type& back() const;

    void clear();

    void push_back(value_type ch);

    // Returns a substring [pos, pos+count). if the requested substring extends past the end of the string, or if count == npos, the returned substring is [pos, size()).
    Utf8String substr(size_type pos, size_type count = npos) const;

    // Returns the number of characters
    size_type size() const noexcept;

    size_type length() const noexcept;

    bool empty() const noexcept;

    // Returns the raw string
    raw_type raw();

    raw_type raw() const noexcept;

    // Returns a non-modifiable standard C character array version of the string
    const char* c_str();

    const char* c_str() const noexcept;


    friend Utf8String operator+(const Utf8String &lhs, const Utf8String &rhs);

    friend bool operator==(const Utf8String &lhs, const Utf8String &rhs);

    friend bool operator!=(const Utf8String &lhs, const Utf8String &rhs);

    friend bool operator<(const Utf8String &lhs, const Utf8String &rhs);

    friend std::ostream& operator<<(std::ostream &out, const Utf8String &str);
    
    friend std::istream& operator>>(std::istream &in, Utf8String &str);
};


extern Utf8String operator+(const Utf8String &lhs, const Utf8String &rhs);

extern bool operator==(const Utf8String &lhs, const Utf8String &rhs);

extern bool operator!=(const Utf8String &lhs, const Utf8String &rhs);

extern bool operator<(const Utf8String &lhs, const Utf8String &rhs);

extern std::ostream& operator<<(std::ostream &out, const Utf8String &str);

extern std::istream& operator>>(std::istream &in, Utf8String &str);


namespace std
{
    template <>
    struct less<Utf8String>
    {
        using result_type = bool;
        using first_argument_type = Utf8String;
        using second_argument_type = Utf8String;

        bool operator()(const Utf8String &lhs, const Utf8String &rhs) const
        {
            return lhs < rhs;
        }
    };
}
