#include <cctype>
#include <fstream>
#include <cassert>
#include "midtopost.hpp"

#define LENGTH 100

using namespace std;

MidtoPost::MidtoPost()
{
    ifstream infile("../resources/priority.conf");
    if (!infile)
        abort();

    string str, pnum;
    while (infile >> str)
    {
        infile >> pnum;
        priority[str] = pnum;
    }
}

int MidtoPost::compare(const stack<string> &oper, const string &ch, int flag3)
{
    if (oper.empty()) return 0;
    else if (ch == "\0" || ch == "!") return -1;
    else if (ch == "(" || ch == "{" || ch == "["
    || (ch == "\\vert" && flag3 != 2)
    || (oper.top() == "\\vert" && ch != "\\vert")
    || (oper.top() == "[" && ch != "]")
    || (oper.top() == "{" && ch != "}")
    || (oper.top() == "(" && ch != ")"))
    {
        return 0;
    }
    else if ((oper.top() == "(" && ch == ")") 
    || (oper.top() == "{" && ch == "}") 
    || (oper.top() == "[" && ch == "]") 
    || (oper.top() == "\\vert" && ch == "\\vert"))
    {
        return 1;
    }
    else if (priority.count(oper.top()) && priority.count(ch))
    {
        return priority[ch] > priority[oper.top()] ? 0 : -1;
    }
    else return -1;
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
    int j = 0;
    int flag, flag2, flag3 = 0;
    int flag1 = 0;
    string opsymbol;
    string opnumber;

#define CHECK if (*str == '\0') break

    while (*str != '\0')
    {
        i = 0;
        flag = 0;
        flag1 = 0;
        while (isspace(*str)) str++;
        while ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || *str == '.')
        {
            temp[i++] = *str++;
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

        while (isspace(*str)) str++;
        if (*str == '\\')
        {
            temp[i] = *str;
            str++;
            i++;
            if (*str == 's')
            {
                while (*str != '{')
                {
                    temp[i++] = *str++;
                    CHECK;
                }
                temp[i] = '\0';
                opsymbol = temp;
                str--;
            }
            else if (*str == 'f')
            {
                while (*str != '{')
                {
                    temp[i++] = *str++;
                    CHECK;
                }
                temp[i] = '\0';
                opsymbol = temp;
                str--;
            }
            else if (*str == 't')
            {
                while (*str != 's')
                {
                    temp[i++] = *str++;
                    CHECK;
                }
                temp[i++] = 's';
                temp[i] = '\0';
                opsymbol = temp;
            }
            else if (*str == 'd')
            {
                while (*str != 'v')
                {
                    temp[i++] = *str++;
                    CHECK;
                }
                temp[i++] = 'v';
                temp[i] = '\0';
                opsymbol = temp;
            }
            else if (*str == 'v')
            {
                while (*str != 't')
                {
                    temp[i++] = *str++;
                    CHECK;
                }
                temp[i++] = 't';
                temp[i] = '\0';
                opsymbol = temp;
            }
            else if (*str == 'n')
            {
                while (*str != 'q')
                {
                    temp[i++] = *str++;
                    CHECK;
                }
                temp[i++] = 'q';
                temp[i] = '\0';
                opsymbol = temp;
            }
            else
            {
                while (*str != '{')
                {
                    temp[i++] = *str++;
                    CHECK;
                }
                temp[i] = '\0';
                opsymbol = temp;
                str--;
            }
        }
        else if (*str == '-')
        {
            str--;
            j = 0;
            flag2 = 0;
            if ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || *str == '.')
            {
                flag2 = 1;
            }
            if (flag2 == 0)
            {
                str++;
                *str = '#';
            }
            else
            {
                while ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || *str == '.')
                {
                    j++;
                    str--;
                }
                if (*str == '\\')
                {
                    flag2 = 1;
                }
                else
                {
                    flag2 = 0;
                }
                while (j)
                {
                    str++;
                    CHECK;
                    j--;
                }
                if (flag2 == 1)
                {
                    str++;
                    CHECK;
                    *str = '#';
                }
                else
                {
                    str++;
                    CHECK;
                    *str = '-';
                }
            }
            temp[i++] = *str;
            temp[i] = '\0';
            opsymbol = temp;
        }
        else if (*str == '+')
        {
            str--;
            j = 0;
            flag2 = 0;
            if ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || *str == '.')
            {
                flag2 = 1;
            }
            if (flag2 == 0)
            {
                str++;
                CHECK;
                *str = '@';
            }
            else
            {
                while ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || *str == '.')
                {
                    j++;
                    str--;
                }
                if (*str == '\\')
                {
                    flag2 = 1;
                }
                else
                {
                    flag2 = 0;
                }
                while (j)
                {
                    str++;
                    j--;
                }
                if (flag2 == 1)
                {
                    str++;
                    CHECK;
                    *str = '@';
                }
                else
                {
                    str++;
                    CHECK;
                    *str = '+';
                }
            }
            temp[i++] = *str;
            temp[i] = '\0';
            opsymbol = temp;
        }
        else
        {
            temp[i++] = *str;
            temp[i] = '\0';
            opsymbol = temp;
        }
        if (flag1 == 1)
        {
            break;
        }
        //cout<<opsymbol<<endl;
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
            if (opsymbol == "\\vert")
            {
                flag3++;
            }
            //cout<<flag3<<endl;
            i = compare(oper, opsymbol, flag3);
            //cout<<i<<endl;
            if (i == 0)
            {
                oper.push(opsymbol);
                //cout<<oper.size()<<endl;
                break;
            }
            else if (i == 1)
            {
                if (flag3 == 2)
                {
                    tree.push_back(oper.top());
                    flag3 = 0;
                }
                oper.pop();
                str++;
                if (*str == '\0')
                {
                    flag1 = 1;
                    opsymbol = "\0";
                }
                else if (*str == '\\')
                {
                    j = 0;
                    temp[i++] = *str++;
                    if (*str == 't')
                    {
                        while (*str != 's')
                        {
                            temp[i++] = *str++;
                            CHECK;
                        }
                        temp[j++] = 's';
                        temp[j] = '\0';
                        opsymbol = temp;
                    }
                    else if (*str == 'd')
                    {
                        while (*str != 'v')
                        {
                            temp[i++] = *str++;
                            CHECK;
                        }
                        temp[j++] = 'v';
                        temp[j] = '\0';
                        opsymbol = temp;
                    }

                    else if (*str == 'n')
                    {
                        while (*str != 'q')
                        {
                            temp[i++] = *str++;
                            CHECK;
                        }
                        temp[j++] = 'q';
                        temp[j] = '\0';
                        opsymbol = temp;
                    }
                }
                else
                {
                    opsymbol = *str;
                }
                //cout<<opsymbol<<endl;
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
            flag3 = 0;
        }

        str++;
    }
    if (!oper.empty())
    {
        tree.push_back(oper.top());
        oper.pop();
    }

    decltype(tree) result;
    for (auto expr : tree)
    {
        if (!expr.empty()) result.push_back(expr);
    }
    return result;
}

list<string> MidtoPost::turntree(const string &input)
{
    static MidtoPost mtp;
    string tmp = input;
    return mtp.turn(tmp);
}
