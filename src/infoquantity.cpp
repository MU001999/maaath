#include <cstring>

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include "infoquantity.hpp"

InfoQuantity::InfoQuantity(std::string filename1, std::string filename2)
{
    std::ifstream fin;
    std::string word;
    double freq, sum;
    fin.open(filename1, std::ios::in);
    if(!fin) std::cout << "Error opening1" << std::endl;
    fin >> word >> sum;
    while(fin >> word >> freq)
    {
        info_quantity[word] = log(sum / freq);
    }
    fin.close();
    fin.open(filename2, std::ios::in);
    if(!fin) std::cout << "Error opening2" << std::endl;
    fin >> word >> sum;
    while(fin >> word >> freq)
    {
        info_quantity[word] = log(sum / freq);
    }
    fin.close();
}

double InfoQuantity::get_infoquantity(std::string word)
{
    for (auto &mp : info_quantity)
        if(word == mp.first) return mp.second;
}

InfoQuantity::~InfoQuantity()
{

}