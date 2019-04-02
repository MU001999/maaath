#include "dse.hpp"

#include <cfloat>
#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

#ifdef DEBUG
#include <cstdio>
#endif // DEBUG


struct _KeywordInfo
{
    double times = 0.0;
    bool is_appeared_in_title = false;
};


FileInfoWithAllKeywords::FileInfoWithAllKeywords(const std::string &filepath) : filepath(filepath) {}


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


std::map<std::string, double> InvertedIndex::cal_scores(const data_type &data)
{
    std::map<std::string, double> scores; // init scores

    for (auto &keyword_fileinfos : data)
    {
        auto &fileinfos = keyword_fileinfos.second;
        int concept_pos = -1;
        for (int i = 0; i < (int)fileinfos.size(); ++i)
        {
            if (fileinfos[i].is_appeared_in_title) 
                scores[fileinfos[(concept_pos = i)].filepath] += 1000;
            else if (concept_pos > -1) 
                scores[fileinfos[i].filepath] -= abs(i - concept_pos) * 100;
            scores[fileinfos[i].filepath] += fileinfos[i].times * InfoQuantity::get_infoquantity(keyword_fileinfos.first);
        }
    }

    return scores;
}


InvertedIndex::InvertedIndex(const std::string &filepath) : tempfilepath(filepath)
{
    serialize();
}

/*
Structure of temp file for serialization
*/

bool InvertedIndex::serialize()
{
    auto fout = std::ofstream(tempfilepath);

    if (!fout) return false;

    for (auto &kv : kwmappings)
    {
        fout << kv.first << std::endl;
        for (auto &file : kv.second)
        {
            fout << file.filepath << std::endl;
            fout << file.times << ' ' << file.density << ' ' << file.is_appeared_in_title << std::endl;
        }
        fout << std::endl;
    }

    return true;
}

bool InvertedIndex::unserialize()
{
    auto fin = std::ifstream(tempfilepath);

    if (!fin)
    {
        add_files();
        return false;
    }

    std::string line, kw;
    while (std::getline(fin, kw))
    {
        while (std::getline(fin, line))
        {
            if (line.empty()) break;

            double times, density;
            bool is_appeared_in_title;

            fin >> times >> density >> is_appeared_in_title;
            kwmappings[kw].push_back({ line, times, density, is_appeared_in_title });
        }
    }

    return true;
}

InvertedIndex::value_type InvertedIndex::get_fileinfos(const key_type &word)
{
    return kwmappings[word];
}

std::vector<FileInfoWithAllKeywords> InvertedIndex::get_fileinfos(const std::vector<key_type> &keywords, int pagenum, int perpage)
{
    data_type kwmps, for_cal_scores;
    std::vector<std::vector<std::string>> filepaths_list;
    for (auto &kw : keywords)
    {
        kwmps[kw] = get_fileinfos(kw);
        filepaths_list.push_back(std::vector<std::string>());
        for (auto &fileinfo : kwmps[kw])
            filepaths_list.back().push_back(fileinfo.filepath);
    }

    std::map<std::string, FileInfoWithAllKeywords> fileinfomps;
    // for (auto filepath : and_files(filepaths_list)) fileinfos[filepath] = filepath;

    for (auto &filepath_fileinfo : fileinfomps)
    {
        for (auto &keyword_fileinfos : kwmps)
        {
            for (auto &fileinfo : keyword_fileinfos.second)
            {
                if (fileinfo.filepath == filepath_fileinfo.first)
                {
                    for_cal_scores[keyword_fileinfos.first].push_back(fileinfo);
                    filepath_fileinfo.second.kwinfos[keyword_fileinfos.first] =
                    {
                        fileinfo.times, fileinfo.density, fileinfo.is_appeared_in_title
                    };
                }
            }
        }
    }

    std::vector<FileInfoWithAllKeywords> fileinfos;
    for (auto &filepath_fileinfo : fileinfomps)
    {
        fileinfos.push_back(filepath_fileinfo.second);
    }

    auto scores = cal_scores(for_cal_scores);
    std::sort(fileinfos.rbegin(), fileinfos.rend(), [&](const FileInfoWithAllKeywords &a, const FileInfoWithAllKeywords &b)
    {
        return scores[a.filepath] < scores[b.filepath];
    });

    return decltype(fileinfos)(fileinfos.begin() + pagenum * perpage - perpage, fileinfos.begin() + pagenum * perpage);
}

std::vector<std::string> InvertedIndex::get_filepaths(const std::vector<key_type> &keywords)
{
    data_type kwmp, for_cal_scores;
    std::vector<std::vector<std::string>> filepaths_list;
    for (auto &kw : keywords)
    {
        kwmp[kw] = get_fileinfos(kw);
        filepaths_list.push_back(std::vector<std::string>());
        for (auto &fileinfo : kwmp[kw])
            filepaths_list.back().push_back(fileinfo.filepath);
    }

    std::vector<std::string> filepaths;
    // auto filepaths = and_files(filepaths_list);

    for (auto &filepath : filepaths)
        for (auto &mp : kwmp)
            for (auto &fileinfo : mp.second)
                if (fileinfo.filepath == filepath)
                    for_cal_scores[mp.first].push_back(fileinfo);

    auto scores = cal_scores(for_cal_scores);
    std::sort(filepaths.rbegin(), filepaths.rend(), [&](const std::string &a, const std::string &b)
    {
        return scores[a] < scores[b];
    });

    return filepaths;
}

// TODO: sort by chapter order & update filesorder
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
            kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
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
                kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
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
            kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
        }
    }

    for (auto &kwinfo : kwinfos)
    {
        auto &info = kwinfo.second;
        kwmappings[kwinfo.first].push_back({ filepath, info.times, info.times / alltimes, info.is_appeared_in_title });
    }
}
