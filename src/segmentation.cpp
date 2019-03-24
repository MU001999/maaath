#include "dse.hpp"

#include <cfloat>
#include <bitset>
#include <iterator>


static double get_infoquantity(const std::vector<Wordmap> &wl)
{
    double info = 0;
    for (auto w : wl)
    {
        if (InfoQuantity::count(w.word)) info += InfoQuantity::get_infoquantity(w.word);
    }
    return info;
}

static decltype(auto) choice_word(const std::vector<Wordmap> &word, std::bitset<32> bits, int count)
{
    std::vector<Wordmap> section;
    for (int i = 0; i < count; i++)
    {
        if (bits[i]) section.push_back(word[i]);
    }
    return section;
}

static bool is_overlapping(const std::vector<Wordmap> &wd)
{
    int word_end_pos = -1;
    for (auto w : wd)
    {
        if (word_end_pos <= w.pos_in_sentence)
            word_end_pos = w.pos_in_sentence + w.word.size();
        else return true;
    }
    return false;
}

static decltype(auto) get_segmentation(const Utf8String &sentence)
{
    std::vector<Wordmap> word_maps;
    for (int i = 0; i < (int)sentence.size() - 1; i++)
    {
        for (int j = 7; j >= 2; j--)
        {
            if (i + j > (int)sentence.size()) j = sentence.size() - i;
            auto temp = sentence.substr(i, j);
            if (InfoQuantity::count(temp)) word_maps.push_back({ temp, i });
        }
    }

#ifdef DEBUG
    std::cout << "[CODE LINE] " << __LINE__ << std::endl;
    for (auto &wm : word_maps)
    {
        std::cout << wm.word.raw() << std::endl;
    }
#endif // DEBUG

    int arrange = -1;
    int count = word_maps.size();
    std::vector<Wordmap> best_segment;

    double freq = DBL_MIN, tmpfreq;
    while (++arrange < (1 << count))
    {
        std::bitset<32> bits(arrange);
#ifdef DEBUG
        std::cout << "[BITS] " << bits << std::endl;
#endif // DEBUG

        auto temp_segment = choice_word(word_maps, bits, count);
        if (!is_overlapping(temp_segment))
        {
            if ((tmpfreq = get_infoquantity(temp_segment)) >= freq)
            {
#ifdef DEBUG
                std::cout << "[FREQ] " << tmpfreq << std::endl;
#endif // DEBUG

                freq = tmpfreq;
                best_segment = temp_segment;
            }
        }
    }

    /* add single character into result
    auto temp_segment = best_segment;
    best_segment.clear();

    int end_pos = -1;
    for (auto &word : temp_segment)
    {
        while (++end_pos < word.pos_in_sentence)
            best_segment.push_back({ sentence[end_pos] , end_pos });
        best_segment.push_back(word);
        end_pos = word.pos_in_sentence + word.word.size() - 1;
    }
    while (++end_pos < (int)sentence.size())
        best_segment.push_back({ sentence[end_pos] , end_pos });
    */

    std::vector<Utf8String> result;
    for (auto &wm : best_segment) result.push_back(wm.word);
    return result;
}


std::vector<Utf8String> Segmentation::segment(const Utf8String &sentence)
{
    return get_segmentation(sentence);
}
