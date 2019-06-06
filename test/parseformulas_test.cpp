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

    TEST_METHOD(parse_formula)
        for (const auto s : Segmentation::get_all_formulas("$x*y^{2}-\\frac{1}{5}*x*y^{2}$"))
            std::cout << s << endl;

        for (const auto s : Segmentation::get_all_formulas("$a ^ b$"))
            std::cout << s << endl;

        for (const auto s : Segmentation::get_all_formulas("$c ^ d$"))
            std::cout << s << endl;
    TEST_METHOD_END()

TEST_CLASS_END()
