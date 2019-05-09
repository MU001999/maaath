#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <map>

typedef std::pair<std::string, int> ITEM;
typedef std::map<std::string, int> TABLE;

class Priority
{
private:
    TABLE table;

public:
    Priority();
    char GetPriority(std::string, std::string);
    bool is_optr(std::string str);
    int get_pri(std::string);
};
