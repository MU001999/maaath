#include <iostream>

#include "common.hpp"

#define _DEBUG

#include "../src/utf8string.cpp"
#include "../src/abstractbuild.cpp"

TEST_CLASS(AbstractBuilder)

    TEST_METHOD(gen_abstract)

    // std::cout << AbstractBuilder::gen_abstract({"有理", "绝对"}, "../resources/Document/Article/1.2.4绝对值") << std::endl;
    std::cout << AbstractBuilder::gen_abstract({ "数轴", "有理", "绝对" }, "../resources/Document/Article/1.3.1有理数的加法") << std::endl;
    std::cout << AbstractBuilder::gen_abstract({ "数轴", "有理", "绝对" }, "../resources/Document/Article/1.2.4绝对值") << std::endl;

    TEST_METHOD_END()

TEST_CLASS_END()