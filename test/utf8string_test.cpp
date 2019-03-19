#include <cstring>
#include <cassert>

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <functional>

#include "../src/utf8string.hpp"


#define TEST(NAME) static int NAME = []{
#define END()      return 0; }();


TEST(CONSTRUCTOR)

END()


int main()
{
    return 0;
}
