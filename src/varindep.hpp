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
    using ITEM = std::pair<std::string, char>;
    using ITERATOR = std::map<std::string, char>::iterator;

    friend class expr_tree;
    char GetSymbol(std::string);
};