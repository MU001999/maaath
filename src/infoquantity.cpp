#include <cmath>

#include <map>
#include <iostream>
#include <fstream>

#include "utf8string.hpp"
#include "infoquantity.hpp"

// Temporary class
static class _InfoQuantity
{
private:
	std::map<Utf8String, double> info_quantity_;

public:

	// Constructor, receives the freq files
	_InfoQuantity(const std::string &filename1 = "../resources/charfreq.txt", const std::string &filename2 = "../resources/wordfreq.txt")
	{
		std::string word;
		double freq, sum;

		std::ifstream fin(filename1);
		if (!fin) std::cout << "Error opening1" << std::endl;
		fin >> word >> sum;
		while (fin >> word >> freq) info_quantity_[word] = log(sum / freq);

		fin.open(filename2);
		if (!fin) std::cout << "Error opening2" << std::endl;
		fin >> word >> sum;
		while (fin >> word >> freq) info_quantity_[word] = log(sum / freq);
	}

	// Destructor
	~_InfoQuantity() {}

	// Returns the infoquantity of given word
	double get_infoquantity(const Utf8String & word)
	{
		return info_quantity_[word];
	}

	// Returns the number of elements matching specific word
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
