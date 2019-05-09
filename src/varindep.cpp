#include "varindep.hpp"

using namespace std;

char VarIndep::GetSymbol(string key)
{
    ITERATOR it = this->dict.find(key);
    if (it != this->dict.end())
    {
        return it->second;
    }
    else
    {
        ITEM temp;
        temp.first = key;
        if (this->dict.empty())
        {
            temp.second = 'A';
        }
        else
        {
            it = this->dict.end();
            temp.second = (char)('A' + (this->lastkey - 'A') + 1);
        }
        this->dict.insert(temp);
        this->lastkey = temp.second;
        return temp.second;
    }
}