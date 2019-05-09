#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <stack>
#include <list>
#include <map>
#include <set>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>

#define LENGTH 100

class MidtoPost
{
private:
    std::stack<std::string> operand;
    std::stack<std::string> oper;
    std::list<std::string> tree;
    std::map<std::string, std::string> priority;

public:
    MidtoPost();
    int compare(std::stack<std::string> oper, const std::string &ch);
    std::list<std::string> turn(std::string &input);
    static std::list<std::string> turntree(const std::string &input);
};