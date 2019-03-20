#pragma once

class InfoQuantity
{
private:
    std::map<std::string, double> info_quantity;

public:

    InfoQuantity(std::string filename1 = "../resources/charfreq.txt", std::string filename2 = "../resources/wordfreq.txt");
    double get_infoquantity(std::string word);
    ~InfoQuantity();
};
