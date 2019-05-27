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
        table[optr] = prty;
    }
}

bool Priority::is_optr(string str)
{
    return table.count(str);
}

int Priority::get_pri(string str)
{
    return table[str];
}
