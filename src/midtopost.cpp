#include "midtopost.hpp"
#include <fstream>

using namespace std;

MidtoPost::MidtoPost()
{
    ifstream infile("../resources/priority.conf");
    if (!infile) abort();

    string str, pnum;
    while (infile >> str)
    {
        infile >> pnum;
        priority.insert({str, pnum});
    }
}

int MidtoPost::compare(stack<string> oper, const string &ch)
{
    map<string, string>::iterator it, its;

    if (!oper.empty())
    {
        it = priority.find(oper.top());
        its = priority.find(ch);
    }
    if (oper.size() == 0)
    {
        return 0;
    }
    else if (ch == "\0" || ch == "!")
    {
        return -1;
    }
    else if (ch == "(" || ch == "{" || (oper.top() == "{" && ch != "}") || (oper.top() == "(" && ch != ")"))
    {
        return 0;
    }

    else if ((oper.top() == "(" && ch == ")") || (oper.top() == "{" && ch == "}"))
    {
        return 1;
    }

    else if (it != priority.end() && its != priority.end())
    {
        if (its->second > it->second)
            return 0;
        else
            return -1;
    }

    else
    {
        return -1;
    }
}

list<string> MidtoPost::turn(string &input)
{
    auto str = input.data();

    stack<string> operand;

    stack<string> oper;
    map<string, string>::iterator maps;
    list<string> tree;
    set<string> opersymbol;
    char temp[100];

    int i = 0;
    int flag;
    int flag1 = 0;
    string opsymbol;
    string opnumber;
    while (*str != '\0')
    {
        i = 0;
        flag = 0;
        flag1 = 0;
        while (*str == ' ')
            str++;
        while ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z'))
        {
            temp[i] = *str;
            str++;
            i++;
            flag = 1;
        }

        if (flag)
        {
            temp[i] = '\0';
            opnumber = temp;
            operand.push(opnumber);
            tree.push_back(opnumber);
        }
        i = 0;

        while (*str == ' ')
            str++;
        if (*str == '\\')
        {
            temp[i] = *str;
            str++;
            i++;
            if (*str == 's')
            {
                while (*str != '{')
                {
                    temp[i] = *str;
                    str++;
                    i++;
                }
                temp[i] = '\0';
                opsymbol = temp;
                str--;
            }
            else if (*str == 'f')
            {
                while (*str != '{')
                {
                    temp[i] = *str;
                    str++;
                    i++;
                }
                temp[i] = '\0';
                opsymbol = temp;
                str--;
            }
            else if (*str == 't')
			{
				while (*str != 's')
				{
					temp[i] = *str;
					str++;
					i++;
				}
				temp[i++] = 's';
				temp[i] = '\0';
				opsymbol = temp;
			}
			else if (*str == 'd')
			{
				while (*str != 'v')
				{
					temp[i++] = *str;
					str++;
				}
				temp[i++] = 'v';
				temp[i] = '\0';
				opsymbol = temp;
			}
            else
            {
                while (*str != '{')
                {
                    temp[i++] = *str;
                    str++;
                }
                temp[i] = '\0';
                opsymbol = temp;
                str--;
            }
        }
        else if (*str == '-')
        {
            str--;
            if ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z'))
            {
                str++;
                *str = '-';
            }
            else
            {
                str++;
                *str = '#';
            }
            temp[i] = *str;
            str++;
            i++;
            temp[i] = '\0';
            opsymbol = temp;
            str--;
        }
        else
        {
            temp[i] = *str;
            str++;
            i++;
            temp[i] = '\0';
            opsymbol = temp;
            str--;
        }
        if (flag1 == 1)
        {
            break;
        }
        maps = priority.find(opsymbol);
        if (maps == priority.end())
        {
            if (opsymbol != "\0")
            {
                break;
            }
        }
        while (1)
        {
            i = compare(oper, opsymbol);
            if (i == 0)
            {
                oper.push(opsymbol);
                break;
            }
            else if (i == 1)
            {
                oper.pop();
                str++;
                if (*str == '\0')
                {
                    flag1 = 1;
                    opsymbol = "\0";
                }
                else
                {
                    opsymbol = *str;
                }
            }
            else if (i == -1)
            {
                tree.push_back(oper.top());
                oper.pop();
                if (flag1 == 1)
                {
                    break;
                }
            }
        }

        str++;
    }
    if (!oper.empty())
    {
        tree.push_back(oper.top());
        oper.pop();
    }

    string kong = "\0";
    list<string>::iterator it;
    for (it = tree.begin(); it != tree.end(); it++)
    {
        if (*it == kong)
        {
            list<string>::iterator tmp = it++;
            tree.erase(tmp);
        }
    }
    return tree;
}

list<string> MidtoPost::turntree(const string &input)
{
    static MidtoPost mtp;
    string tmp = input;
    return mtp.turn(tmp);
}
