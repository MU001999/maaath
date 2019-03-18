template <std::size_t size_of_bits>
class Utf8Char
{
private:
    std::bitset<size_of_bits> value;

public:
    Utf8Char()
    {

    }

    std::size_t size()
    {
        return size_of_bits;
    }
};