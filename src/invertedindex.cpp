#include "dse.hpp"

#include <cfloat>
#include <bitset>
#include <iterator>

#ifdef DEBUG
#include <cstdio>
#include <iostream>
#endif // DEBUG


struct _KeywordInfo
{
    double times = 0.0;
    bool is_appeared_in_title = false;
};

static std::map<InvertedIndex::key_type, InvertedIndex::value_type> files;

static decltype(auto) get_ambiguity_section(const Utf8String &sentence)
{
    int pos_of_sen = 0, temp_pos = 0, end_of_sen = 0;
    std::map<int, int> ambiguity;

    for (int pos = 0, length; pos < (int)sentence.size() - 1;)
    {
        for (length = 7; length > 1; --length)
        {
            auto tmp = sentence.substr(pos, length);
            if (!InfoQuantity::count(tmp)) continue;
            for (int anpos = pos + length - 1, anlen; anpos > pos; --anpos)
            {
                for (anlen = 7; anpos + anlen > pos + length; --anlen)
                {
                    auto antmp = sentence.substr(anpos, anlen);
                    if (!InfoQuantity::count(antmp)) continue;
                    ambiguity[pos] = anpos + anlen;
                    anpos = pos;
                    break;
                }
            }
            break;
        }
        pos += length;
    }

    return ambiguity;
}


bool InvertedIndex::serialize()
{
    // TODO: implement serialize
    return true;
}

bool InvertedIndex::unserialize()
{
    // TODO: implement unserialize
    return false;
}

InvertedIndex::value_type InvertedIndex::get_files_list(const key_type &word)
{
    return files[word];
}

void InvertedIndex::add_files(const std::string &folderpath)
{
    // TODO: implement add files with receiving a folder path
    serialize();
}

void InvertedIndex::add_file(const key_type &sentence, const std::string &filepath)// Add all the words in a sentence to the inverted index
{
    double alltimes = 0.0;

    std::map<key_type, _KeywordInfo> kwinfos;

    auto ambiguities = get_ambiguity_section(sentence);
    for (auto &mp : ambiguities)
    {
        for (auto &word : Segmentation::segment(sentence.substr(mp.first, mp.second - mp.first)))
        {
            alltimes += 1;
            kwinfos[word].times += 1;
            kwinfos[word].is_appeared_in_title |= false;
        }
    }

    int endpos = -1;
    for (auto &mp : ambiguities)
    {
        while (++endpos < mp.first - 1)
        {
            for (int length = 2; endpos + length < mp.first; ++length)
            {
                auto word = sentence.substr(endpos, length);
                if (!InfoQuantity::count(word)) continue;
                alltimes += 1;
                kwinfos[word].times += 1;
                kwinfos[word].is_appeared_in_title |= false;
            }
        }
        endpos = mp.second - 1;
    }
    while (++endpos < (int)sentence.size() - 1)
    {
        for (int length = 2; endpos + length < (int)sentence.size(); ++length)
        {
            auto word = sentence.substr(endpos, length);
            if (!InfoQuantity::count(word)) continue;
            alltimes += 1;
            kwinfos[word].times += 1;
            kwinfos[word].is_appeared_in_title |= false;
        }
    }

    for (auto &kwinfo : kwinfos)
    {
        auto &info = kwinfo.second;
        files[kwinfo.first].push_back({ filepath, info.times, info.times / alltimes, info.is_appeared_in_title });
    }
}
