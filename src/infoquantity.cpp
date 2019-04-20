#include <cmath>

#include <map>
#include <fstream>
#include <iostream>

#include "utf8string.hpp"
#include "infoquantity.hpp"

// #define _DEBUG
#ifdef _DEBUG
#include <cstdio>
#include <iostream>
#endif


// temporary class
static class _InfoQuantity
{
private:
	std::map<Utf8String, double> info_quantity_;

public:

	// constructor, receives the freq files
	_InfoQuantity(const std::string &filename1 = "../resources/charfreq.txt", const std::string &filename2 = "../resources/wordfreq.txt")
	{
		std::string word;
		double freq, sum;

#ifdef _DEBUG
        printf("[InfoQuantity] [Contructor] [charfreq.txt]\n");
#endif

		std::ifstream fin(filename1);
		if (!fin) abort();
		fin >> word >> sum;
		while (fin >> word >> freq) info_quantity_[word] = log(sum / freq);
        fin.close();

#ifdef _DEBUG
        printf("[InfoQuantity] [Contructor] [wordfreq.txt]\n");
#endif

		fin.open(filename2);
		if (!fin) abort();
		fin >> word >> sum;
		while (fin >> word >> freq) info_quantity_[word] = log(sum / freq);
	}

	// destructor
	~_InfoQuantity() {}

	// returns the infoquantity of given word
	double get_infoquantity(const Utf8String & word)
	{
		return info_quantity_[word];
	}

	// returns the number of elements matching specific word
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
