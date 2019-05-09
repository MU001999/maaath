#pragma once

#include <iostream>
#include <string>
#include <map>

class VarIndep
{
private:
    std::map<std::string, char> dict;
    char lastkey;

public:
    friend class expr_tree;
    char GetSymbol(std::string);
    typedef std::pair<std::string, char> ITEM;
    typedef std::map<std::string, char>::iterator ITERATOR;
};