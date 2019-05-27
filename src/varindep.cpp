#include "varindep.hpp"

using namespace std;

char VarIndep::GetSymbol(string key)
{
    if (dict.count(key)) return dict[key];
    else
    {
        auto value = dict.empty() ? 'A' : (lastkey + 1);
        dict[key] = lastkey = value;
        return value;
    }
}