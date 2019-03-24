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


InvertedIndex::value_type InvertedIndex::get_files_list(const key_type &word)
{
    return files[word];
}

void InvertedIndex::add_file(const key_type &sentence, int file_id)// Add all the words in a sentence to the inverted index
{
    auto ambiguity_section = get_ambiguity_section(sentence);
    for (auto &mp : ambiguity_section)
    {
        for (auto &wordmap : Segmentation::segment(sentence.substr(mp.first, mp.second - mp.first + 1)))
        {
            if (wordmap.word.size() >= 2) 
                files[wordmap.word].push_back(file_id);
        }
    }

    int se_begin = 0, se_end;
    for (auto &mp : ambiguity_section)
    {
        if (mp.first > se_begin)
        {
            se_end = mp.first;
            auto temp = sentence.substr(se_begin, se_end - se_begin);
            int pos = 0, i;
            while (pos < (int)temp.size())
            {
                for (i = 8; i >= 2; --i)
                {
                    auto kw = temp.substr(pos, i);
                    if (InfoQuantity::count(kw))
                    {
                        files[kw].push_back(file_id);
                        pos += i;
                        break;
                    }
                }
                pos += (i == 1);
            }
        }
        se_begin = mp.second + 1;
    }

    for (int i = se_begin, j; i < (int)sentence.size();)
    {
        for (j = 8; j >= 2; j--)
        {
            auto temp = sentence.substr(i, j);
            if (InfoQuantity::count(temp) > 0)
            {
                files[temp].push_back(file_id);
                i += j;
                break;
            }
        }
        i += (j == 1);
    }
}
