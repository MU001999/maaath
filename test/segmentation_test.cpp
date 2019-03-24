#include <iostream>

// #define DEBUG

#include "../src/utf8string.cpp"
#include "../src/infoquantity.cpp"
#include "../src/invertedindex.cpp"


#define TEST(NAME) static int NAME = []{
#define END()      return 0; }();


TEST(GETSEGMENT)
    for (auto &s : Segmentation::segment("记者读报道的来源是呱呱食堂")) 
        std::cout << s.word.raw() << std::endl;

    for (auto &s : Segmentation::segment("要长大，要可爱，但不要变成各种奇奇怪怪的不可爱的人"))
        std::cout << s.word.raw() << std::endl;
END()


int main()
{
    return 0;
}
