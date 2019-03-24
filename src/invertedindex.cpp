#include"dse.hpp"

#include <cfloat>
#include <bitset>
#include <iterator>

#ifdef DEBUG
#include <cstdio>
#include <iostream>
#endif // DEBUG


static decltype(auto) get_ambiguity_section(const Utf8String &sentence)
{
    double freq;
    int pos_of_sen = 0, temp_pos = 0, end_of_sen = 0;
    std::map<int, int> ambiguity;
    while (pos_of_sen < (int)sentence.size())
    {
        Utf8String temp;
        int i;
        for (i = 8; i > 1; i--)
        {
            temp = sentence.substr(pos_of_sen, i);
            freq = InfoQuantity::get_infoquantity(temp);

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
        while (temp_pos <= end_of_sen && temp_pos < (int)sentence.size())
        {
            for (i = 8; i > 1; i--)
            {
                temp = sentence.substr(temp_pos, i);
                freq = InfoQuantity::get_infoquantity(temp);
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
    auto t = ambiguity.end();
    t--;
    if (t->second >= (int)sentence.size())
        t->second = sentence.size();
    return ambiguity;
}


InvertedIndex::InvertedIndex()
{
    // TODO: Check if the serialization file exists, initialize the object
};

std::set<int> InvertedIndex::get_file_list(const Utf8String &word)
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

void InvertedIndex::add_file(const Utf8String &sentence, int file_id)// Add all the words in a sentence to the inverted index
{
    std::vector<Wordmap> word_list;
    std::map<int, int> ambiguity_section;
    ambiguity_section = get_ambiguity_section(sentence);
    Utf8String segment;
    std::map<int, int>::iterator t = ambiguity_section.begin();
    while (t != ambiguity_section.end())
    {
        segment = sentence.substr(t->first, t->second - t->first + 1);
        word_list = Segmentation::segment(segment);
        for (int i = 0; i < (int)word_list.size(); i++)
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
            while (pos < (int)temp.size())
            {
                for (i = 8; i >= 2; i--)
                {
                    if (InfoQuantity::get_infoquantity(temp.substr(pos, i)) > 0)
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
    if (se_begin < (int)sentence.size())
    {
        int j;
        for (int i = se_begin; i < (int)sentence.size();)
        {
            for (j = 8; j >= 2; j--)
            {
                Utf8String temp = sentence.substr(i, j);
                if (InfoQuantity::get_infoquantity(temp) > 0)
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
