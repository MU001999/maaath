#include <cfloat>

#include <bitset>
#include <iterator>

#include "utf8string.hpp"
#include "infoquantity.hpp"
#include "segmentation.hpp"


// struct records word and its position start in its sentence
struct _Wordmap
{
	Utf8String word;
	int pos_in_sentence;
};


// returns summary of infoquantities by given words
static double _cal_infoquantity_of_words(const std::vector<_Wordmap>& wl)
{
	double res = 0.0;
	for (auto &w : wl)
		if (InfoQuantity::count(w.word)) // if word in given freq files
			res += InfoQuantity::get_infoquantity(w.word);
	return res;
}

// returns vector<_Wordmap> by given bits
static decltype(auto) _choice_word(const std::vector<_Wordmap>& word, std::bitset<32> bits, int count)
{
	std::vector<_Wordmap> res;
	for (int i = 0; i < count; i++)
		if (bits[i]) // if it's chosen
			res.push_back(word[i]);
	return res;
}

// checks it's overlapping or not
static bool _is_overlapping(const std::vector<_Wordmap>& wd)
{
	int word_end_pos = -1;
	for (auto &w : wd)
	{
		if (word_end_pos <= w.pos_in_sentence)
			word_end_pos = w.pos_in_sentence + w.word.size();
		else return true; // is overlapping
	}
	return false;
}

// real segment function, returns segmentation result from given sentence
static decltype(auto) _get_segmentation(const Utf8String & sentence)
{
	std::vector<_Wordmap> word_maps;
	for (int i = 0; i < (int)sentence.size() - 1; i++)
	{
		// Enumerate all possible words
		for (int j = 7; j >= 2; j--)
		{
			if (i + j > (int)sentence.size()) j = sentence.size() - i;
			auto temp = sentence.substr(i, j);
			if (InfoQuantity::count(temp)) word_maps.push_back({ temp, i });
		}
	}

	int arrange = -1;
	int count = word_maps.size();
	std::vector<_Wordmap> best_segment;

	double freq = DBL_MIN, tmpfreq;
	while (++arrange < (1 << count))
	{
		std::bitset<32> bits(arrange);

		auto temp_segment = _choice_word(word_maps, bits, count);
		if (!_is_overlapping(temp_segment))
		{
			if ((tmpfreq = _cal_infoquantity_of_words(temp_segment)) >= freq)
			{
				freq = tmpfreq;
				best_segment = temp_segment;
			}
		}
	}

	std::vector<Utf8String> result;
	for (auto& wm : best_segment) result.push_back(wm.word);
	return result;
}


std::vector<Utf8String> Segmentation::segment(const Utf8String & sentence)
{
	return _get_segmentation(sentence);
}
