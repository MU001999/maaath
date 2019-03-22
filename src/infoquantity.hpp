#pragma once

class InfoQuantity
{
private:
    std::map<Utf8String, double> info_quantity;

public:

    InfoQuantity(Utf8String filename1 = "../resources/charfreq.txt", Utf8String filename2 = "../resources/wordfreq.txt");
    ~InfoQuantity();

    double get_infoquantity(Utf8String word);

    bool count(Utf8String word);
};
