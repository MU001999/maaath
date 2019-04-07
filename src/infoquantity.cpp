#include "dse.hpp"

#include <cmath>
#include <cstring>

#include <map>
#include <string>
#include <fstream>


static class _InfoQuantity
{
private:
	std::map<Utf8String, double> info_quantity_;

public:

	_InfoQuantity(Utf8String filename1 = "../resources/charfreq.txt", Utf8String filename2 = "../resources/wordfreq.txt")
	{
		std::ifstream fin;
		std::string word;
		double freq, sum;
		fin.open(filename1.raw(), std::ios::in);
		if (!fin) std::cout << "Error opening1" << std::endl;
		fin >> word >> sum;
		while (fin >> word >> freq)
		{
			info_quantity_[word] = log(sum / freq);
		}
		fin.close();
		fin.open(filename2.raw(), std::ios::in);
		if (!fin) std::cout << "Error opening2" << std::endl;
		fin >> word >> sum;
		while (fin >> word >> freq)
		{
			info_quantity_[word] = log(sum / freq);
		}
		fin.close();
	}
	~_InfoQuantity() {}

	double get_infoquantity(const Utf8String & word)
	{
		return info_quantity_[word];
	}

	bool count(const Utf8String & word) const
	{
		return info_quantity_.count(word);
	}
} _infoquantity;


double InfoQuantity::get_infoquantity(const Utf8String & word)
{
	return _infoquantity.get_infoquantity(word);
}

bool InfoQuantity::count(const Utf8String & word)
{
	return _infoquantity.count(word);
}
