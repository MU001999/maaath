#include <iostream>

// #define DEBUG

#include "../src/utf8string.cpp"
#include "../src/infoquantity.cpp"
#include "../src/invertedindex.cpp"


#define TEST(NAME) static int NAME = []{
#define END()      return 0; }();


TEST(GETSEGMENT)
    auto res = get_segmentation("记者读报道的来源是呱呱食堂", InfoQuantity());
    for (auto s : res) std::cout << s.word.raw() << std::endl;
END()


int main()
{
    return 0;
}
