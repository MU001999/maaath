#include "dse.hpp"

#include <cstring>

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

InfoQuantity::InfoQuantity(Utf8String filename1, Utf8String filename2)
{
    std::ifstream fin;
    std::string word;
    double freq, sum;
    fin.open(filename1.raw(), std::ios::in);
    if (!fin) std::cout << "Error opening1" << std::endl;
    fin >> word >> sum;
    while (fin >> word >> freq)
    {
        info_quantity[word] = log(sum / freq);
    }
    fin.close();
    fin.open(filename2.raw(), std::ios::in);
    if (!fin) std::cout << "Error opening2" << std::endl;
    fin >> word >> sum;
    while (fin >> word >> freq)
    {
        info_quantity[word] = log(sum / freq);
    }
    fin.close();
}

InfoQuantity::~InfoQuantity() {}

double InfoQuantity::get_infoquantity(Utf8String word)
{
    return info_quantity[word];
}

bool InfoQuantity::count(Utf8String word)
{
    return info_quantity.count(word);
}