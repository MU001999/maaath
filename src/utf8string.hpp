class Utf8String
{
public:
    using value_type = char32_t;
    using raw_type = std::string;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type&;

private:
    std::vector<value_type> data;
    raw_type raw_string;

public:
    static const size_type npos = -1;

    // Constructor, receives std::string
    Utf8String(const raw_type &raw_string);

    Utf8String(const Utf8String &rhs);

    Utf8String(Utf8String &&rhs) noexcept;

    ~Utf8String();

    Utf8String& operator=(const raw_type &raw_string);

    Utf8String& operator=(const Utf8String &rhs);

    Utf8String& operator=(Utf8String &&rhs) noexcept;

    reference operator[](std::size_t index);

    const_reference operator[](std::size_t index) const;

    // Returns a substring [pos, pos+count). if the requested substring extends past the end of the string, or if count == npos, the returned substring is [pos, size()).
    Utf8String substr(size_type pos = 0, size_type count = npos) const;

    const size_type size() const noexcept;

    raw_type raw() const noexcept;

    const char* c_str() const noexcept;

    friend bool operator==(const Utf8String &lhs, const Utf8String &rhs);

    friend bool operator<(const Utf8String &lhs, const Utf8String &rhs);
};


extern bool operator==(const U8S &lhs, const Utf8String &rhs);

extern bool operator<(const Utf8String &lhs, const Utf8String &rhs);


template <>
class std::less<Utf8String>
{
public:
    using result_type = bool;
    using first_argument_type = Utf8String;
    using second_argument_type = Utf8String;

    bool operator()(const Utf8String &lhs, const Utf8String &rhs) const
    {
        return lhs < rhs;
    }
};
