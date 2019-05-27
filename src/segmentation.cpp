#include <cfloat>

#include <bitset>
#include <iterator>

#include "utf8string.hpp"
#include "infoquantity.hpp"
#include "segmentation.hpp"
#include "tnode.hpp"
#include "priority.hpp"
#include "varindep.hpp"
#include "midtopost.hpp"

// #define _DEBUG
#ifdef _DEBUG
#include <cstdio>
#include <iostream>
#endif

namespace
{
// struct records word and its position start in its sentence
struct _Wordmap
{
    Utf8String word;
    int pos_in_sentence;
};

// add single character into result
std::vector<_Wordmap> continuitify(const std::vector<_Wordmap>& wl, const Utf8String &sentence)
{
    std::vector<_Wordmap> result;
    int end_pos = -1;
    for (auto &word : wl)
    {
        while (++end_pos < word.pos_in_sentence)
            result.push_back({ sentence[end_pos] , end_pos });
        result.push_back(word);
        end_pos = word.pos_in_sentence + word.word.size() - 1;
    }
    while (++end_pos < (int)sentence.size())
        result.push_back({ sentence[end_pos] , end_pos });
    return result;
} 

// returns summary of infoquantities by given words
double cal_infoquantity_of_words(const std::vector<_Wordmap>& wl)
{
    double res = 0.0;
    for (auto &w : wl)
        if (InfoQuantity::count(w.word)) // if word in given freq files
            res += InfoQuantity::get_infoquantity(w.word);
    return res;
}

// returns vector<_Wordmap> by given bits
decltype(auto) choice_word(const std::vector<_Wordmap>& word, std::bitset<32> bits, int count)
{
    std::vector<_Wordmap> res;
    for (int i = 0; i < count; i++)
        if (bits[i]) // if it's chosen
            res.push_back(word[i]);
    return res;
}

// checks it's overlapping or not
bool is_overlapping(const std::vector<_Wordmap>& wd)
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
decltype(auto) get_segmentation(const Utf8String & sentence)
{
    std::vector<_Wordmap> word_maps;
    for (int i = 0; i < (int)sentence.size() - 1; i++)
    {
        // enumerate all possible words
        for (int j = 7; j >= 2; j--)
        {
            if (i + j > (int)sentence.size()) j = sentence.size() - i;
            auto temp = sentence.substr(i, j);
            if (InfoQuantity::count(temp)) word_maps.push_back({ temp, i });
        }
    }

#ifdef _DEBUG
    std::cout << "[CODE LINE] " << __LINE__ << std::endl;
    for (auto& wm : word_maps)
    {
        std::cout << wm.word << " ";
    }
    std::cout << std::endl;
#endif // _DEBUG

    int arrange = -1;
    int count = word_maps.size();
    std::vector<_Wordmap> best_segment;

    double freq = DBL_MAX, tmpfreq;
    while (++arrange < (1 << count))
    {
        std::bitset<32> bits(arrange);

#ifdef _DEBUG
        std::cout << "[BITS] " << bits << std::endl;
#endif // _DEBUG

        auto temp_segment = choice_word(word_maps, bits, count);
        if (!is_overlapping(temp_segment))
        {
            if ((tmpfreq = cal_infoquantity_of_words(continuitify(temp_segment, sentence))) <= freq)
            {
#ifdef _DEBUG
                std::cout << "[FREQ] " << tmpfreq << std::endl;
#endif // _DEBUG
                freq = tmpfreq;
                best_segment = temp_segment;
            }
        }
    }

    best_segment = continuitify(best_segment, sentence);

    std::vector<Utf8String> result;
    for (auto& wm : best_segment) result.push_back(wm.word);
    return result;
}
} // namespace

std::vector<Utf8String> Segmentation::segment(const Utf8String & sentence)
{
    return get_segmentation(sentence);
}

std::list<std::string> Segmentation::get_all_formulas(const std::string &formula)
{
#ifdef _DEBUG
    std::cout << "[GET FORMULAS] " << formula << std::endl;
#endif // _DEBUG
    auto len = Utf8String(formula).size();
    if (len < formula.size() || len < 3) return {};

    std::list<std::string> formulas;

    static Priority priority;
    auto mylist = MidtoPost::turntree(formula.substr(1, formula.size() - 2));

    expr_tree tree;
    VarIndep a;
    tree.parse_expr(mylist, &priority);
    tree.show(&priority, &a, formulas);

    return formulas;
}
