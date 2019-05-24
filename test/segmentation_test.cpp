#include <iostream>

#include "common.hpp"

// #define _DEBUG

#include "../src/utf8string.cpp"
#include "../src/infoquantity.cpp"
#include "../src/segmentation.cpp"
#include "../src/midtopost.cpp"
#include "../src/priority.cpp"
#include "../src/tnode.cpp"
#include "../src/varindep.cpp"


TEST_CLASS(Segmentation)

    TEST_METHOD(segment)
        for (auto& s : Segmentation::segment("记者读报道的来源是呱呱食堂"))
            std::cout << s.raw() << " ";
        std::cout << std::endl;

        for (auto& s : Segmentation::segment("要长大，要可爱，但不要变成各种奇奇怪怪的不可爱的人"))
            std::cout << s.raw() << " ";
        std::cout << std::endl;

        for (auto& s : Segmentation::segment("有理数的数轴很神奇，它有好多个小点点"))
            std::cout << s.raw() << " ";
        std::cout << std::endl;
    TEST_METHOD_END()

TEST_CLASS_END()