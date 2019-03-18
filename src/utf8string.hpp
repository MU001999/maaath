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
    Utf8String(const raw_type &raw_string);

    Utf8String(const Utf8String &rhs);

    Utf8String(Utf8String &&rhs) noexcept;

    ~Utf8String();

    Utf8String& operator=(const raw_type &raw_string);

    Utf8String& operator=(const Utf8String &rhs);

    Utf8String& operator=(Utf8String &&rhs) noexcept;

    const size_type size() const noexcept;

    raw_type raw() const noexcept;

    const char* c_str() const noexcept;
};


extern bool operator==(const Utf8String &lhs, const Utf8String &rhs);
