#include "priority.hpp"

using namespace std;

Priority::Priority()
{
    ifstream infile("../resources/OperatorPriority.conf");
    if (!infile) abort();

    string optr; int prty;
    while (infile >> optr)
    {
        infile >> prty;
        ITEM item(optr, prty);
        this->table.insert(item);
    }
}

char Priority::GetPriority(string optr1, string optr2)
{
    int result1, result2;
    TABLE::iterator it;

    it = this->table.find(optr1);
    if (it != this->table.end()) result1 = it->second;
    else return '\0';

    it = this->table.find(optr2);
    if (it != this->table.end()) result2 = it->second;
    else return '\0';

    if (result1 - result2 > 0) return '>';
    else if (result1 - result2 < 0) return '<';
    else return '=';
}

bool Priority::is_optr(string str)
{
    TABLE::iterator it;

    it = this->table.find(str);
    return it != this->table.end();
}

int Priority::get_pri(string str)
{
    TABLE::iterator it;
    it = this->table.find(str);
    return it->second;
}
