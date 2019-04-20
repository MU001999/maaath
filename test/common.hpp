#pragma once

#include <cassert>

#define TEST_CLASS(NAME)    namespace CLASS##NAME {
#define TEST_CLASS_END()    }
#define TEST_METHOD(NAME)   static int METHOD##NAME = [] {
#define TEST_METHOD_END()   return 0; }();


int main(int argc, char* agrv[])
{
    return 0;
}