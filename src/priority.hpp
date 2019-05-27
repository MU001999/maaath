#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <map>

class Priority
{
private:
    std::map<std::string, int> table;

public:
    Priority();
    bool is_optr(std::string str);
    int get_pri(std::string);
};
