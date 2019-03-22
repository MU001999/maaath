#include"dse.hpp"

#include <cfloat>
#include <bitset>
#include <iterator>

#ifdef DEBUG
#include <cstdio>
#include <iostream>
#endif // DEBUG


#define BIT 0xffffffff


struct wordmap
{
    Utf8String word;
    int pos_in_sentence;
};


static std::map<int, int> get_ambiguity_section(Utf8String sentence, InfoQuantity dict)
{
    int pos_of_sen = 0;
    int temp_pos = 0;
    int end_of_sen = 0;
    std::map<int, int>ambiguity;
    double freq;
    while (pos_of_sen < sentence.size())
    {
        Utf8String temp;
        int i;
        for (i = 8; i > 1; i--)
        {

            temp = sentence.substr(pos_of_sen, i);
            freq = dict.get_infoquantity(temp);

            if (freq > 0)
            {
                end_of_sen = i + pos_of_sen - 1;
                break;
            }
        }
        if (i == 1)
        {
            pos_of_sen++;
            continue;
        }
        temp_pos = pos_of_sen + 1;
        while (temp_pos <= end_of_sen && temp_pos < sentence.size())
        {
            for (i = 8; i > 1; i--)
            {
                temp = sentence.substr(temp_pos, i);
                freq = dict.get_infoquantity(temp);
                if (freq > 0)
                {
                    if (temp_pos + i - 1 > end_of_sen)
                    {
                        end_of_sen = temp_pos + i - 1;
                        temp_pos++;
                        break;
                    }
                }
            }

            temp_pos++;
        }
        ambiguity[pos_of_sen] = end_of_sen;
        pos_of_sen = end_of_sen + 1;

    }
    std::map<int, int>::iterator t = ambiguity.end();
    t--;
    if (t->second >= sentence.size())
        t->second = sentence.size();
    return ambiguity;
}

static double get_infoquantity(std::vector<wordmap>wl, InfoQuantity dict)
{
    double info = 0;
    for (auto w : wl)
    {
        info += dict.get_infoquantity(w.word);
    }
    return info;
}

static void choice_word(std::vector<wordmap>word, std::bitset<32>bits, int count, std::vector<wordmap>&section)
{
    for (int i = 0; i < count; i++) {
        if (bits[i]) {
            section.push_back(word[i]);
        }
    }
}

static bool is_overlapping(std::vector<wordmap>wd)
{
    int word_end_pos = -1;
    for (auto w: wd)
    {
        if (word_end_pos <= w.pos_in_sentence) 
            word_end_pos = w.pos_in_sentence + w.word.size();
        else return true;
    }
    return false;
}

std::vector<wordmap> get_segmentation(Utf8String sentence, InfoQuantity dict)
{
    wordmap word_map;
    Utf8String temp;
    std::vector<wordmap> word_map_dict;
    for (int i = 0; i < sentence.size() - 1; i++) {
        if (sentence.size() - i <= 2) {
            temp = sentence.substr(i, 2);
            if (dict.count(temp))
                word_map.pos_in_sentence = i;
            word_map.word = temp;
            word_map_dict.push_back(word_map);
            break;
        }
        for (int j = 7; j >= 2; j--)
        {
            temp = sentence.substr(i, j);
            if (dict.count(temp))
            {
                word_map.pos_in_sentence = i;
                word_map.word = temp;
                word_map_dict.push_back(word_map);
            }
        }
    }

#ifdef DEBUG
    std::cout << "[CODE LINE] " << __LINE__ << std::endl;
    for (auto &wm : word_map_dict)
    {
        std::cout << wm.word.raw() << std::endl;
    }
#endif // DEBUG

    int count = word_map_dict.size();
    int arrange = -1;
    std::vector<wordmap>temp_segment;
    std::vector<wordmap>best_segment;
    /*
    for (int i = 0; i < sentence.size(); i++) {
        wordmap tem;
        tem.word = sentence.substr(i, 1);
        tem.pos_in_sentence = i;
        best_segment.push_back(tem);
    }
    double freq = get_infoquantity(best_segment, dict);
    */
    double freq = DBL_MIN, tmpfreq;
    while (++arrange < (1 << count))
    {
        std::bitset<32> bits(arrange);
#ifdef DEBUG
        std::cout << "[BITS] " << bits << std::endl;
#endif // DEBUG

        temp_segment.clear();
        choice_word(word_map_dict, bits, count, temp_segment);
        if (!is_overlapping(temp_segment))
        {
            if ((tmpfreq = get_infoquantity(temp_segment, dict)) >= freq)
            {
#ifdef DEBUG
                std::cout << "[FREQ] " << tmpfreq << std::endl;
#endif // DEBUG

                freq = tmpfreq;
                best_segment = temp_segment;
            }
        }
    }

    {
        auto temp_segment = best_segment;
        best_segment.clear();

        int end_pos = -1;
        for (auto &word : temp_segment)
        {
            while (++end_pos < word.pos_in_sentence)
                best_segment.push_back({ sentence.substr(end_pos, 1) , end_pos });
            best_segment.push_back(word);
            end_pos = word.pos_in_sentence + word.word.size() - 1;
        }
        while (++end_pos < sentence.size())
            best_segment.push_back({ sentence.substr(end_pos, 1) , end_pos });
    }

    return best_segment;
}


InvertedIndex::InvertedIndex()
{
    // TODO: Check if the serialization file exists, initialize the object
};

std::set<int> InvertedIndex::get_file_list(Utf8String word)
{
    std::map<Utf8String, std::set<int>>::iterator t;
    t = this->index.find(word);
    if (t != this->index.end())
        return t->second;
    else
    {
        std::set<int> empty_file_list;
        empty_file_list.clear();
        return empty_file_list;
    }
}

void InvertedIndex::add_file(Utf8String sentence, int file_id, InfoQuantity dict)// Add all the words in a sentence to the inverted index
{
    std::vector<wordmap> word_list;
    std::map<int, int> ambiguity_section;
    ambiguity_section = get_ambiguity_section(sentence, dict);
    Utf8String segment;
    std::map<int, int>::iterator t = ambiguity_section.begin();
    while (t != ambiguity_section.end())
    {
        segment = sentence.substr(t->first, t->second - t->first + 1);
        word_list = get_segmentation(segment, dict);
        for (int i = 0; i < word_list.size(); i++)
        {
            Utf8String temp = word_list[i].word;
            if (temp.size() >= 2)
                this->index[temp].insert(file_id);
        }
        t++;
        word_list.clear();
    }
    t = ambiguity_section.begin();
    int se_begin = 0;
    int se_end;
    while (t != ambiguity_section.end())
    {
        if (t->first > se_begin)
        {
            se_end = t->first;
            Utf8String temp = sentence.substr(se_begin, se_end - se_begin);
            int pos = 0;
            int i;
            while (pos < temp.size())
            {
                for (i = 8; i >= 2; i--)
                {
                    if (dict.get_infoquantity(temp.substr(pos, i)) > 0)
                    {
                        this->index[temp.substr(pos, i)].insert(file_id);
                        pos = pos + i;
                        break;
                    }
                }
                if (i == 1) pos++;
            }

        }
        se_begin = t->second + 1;
        t++;
    }
    if (se_begin < sentence.size())
    {
        int j;
        for (int i = se_begin; i < sentence.size();)
        {
            for (j = 8; j >= 2; j--)
            {
                Utf8String temp = sentence.substr(i, j);
                if (dict.get_infoquantity(temp) > 0)
                {
                    this->index[temp.substr(i, j)].insert(file_id);
                    i = i + j;
                    break;
                }
            }
            if (j == 1)i++;
        }
    }

}
