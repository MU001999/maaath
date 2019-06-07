#include <cstdio>
#include <cfloat>
#include <cctype>
#include <cstdlib>

#include <bitset>
#include <fstream>
#include <sstream>
#include <iterator>
#include <streambuf>
#include <algorithm>
#include <filesystem>

#include "utf8string.hpp"
#include "infoquantity.hpp"
#include "segmentation.hpp"
#include "invertedindex.hpp"

// #define _DEBUG
#ifdef _DEBUG
#include <cstdio>
#include <iostream>
#endif

namespace fs = std::filesystem;

namespace
{
// temporary struct in this file
struct _KeywordInfo
{
    double times = 0.0;
    bool is_appeared_in_title = false;
};

std::vector<std::string> combine(std::vector<std::vector<std::string>> paths_list)
{
    if (paths_list.empty()) return {};

    sort(paths_list.begin(), paths_list.end(), [](const std::vector<std::string> & a, const std::vector<std::string> & b) { return a.size() < b.size(); });
    for (auto& paths : paths_list) std::sort(paths.begin(), paths.end());
    auto result = paths_list.front();

    for (const auto& paths : paths_list)
    {
        decltype(result) tmp;
        for (size_t i = 0, j = 0; i < paths.size() && j < result.size();)
        {
            if (paths[i] == result[j])
            {
                tmp.push_back(paths[i++]);
                ++j;
            }
            else if (paths[i] < result[j]) ++i;
            else ++j;
        }
        result = tmp;
    }

    return result;
}

// returns map for ambiguity section by given sentence
decltype(auto) get_ambiguity_section(const Utf8String& sentence)
{
    std::map<int, int> ambiguity;

    for (int pos = 0, length; pos < (int)sentence.size() - 1;)
    {
        if (sentence[pos] == '$')
        {
            auto end_pos = sentence.find('$', pos + 1);
            if (end_pos == sentence.npos) break;
            else ambiguity[pos] = end_pos + 1;
        }
        else for (length = 7; length > 1; --length)
        {
            auto tmp = sentence.substr(pos, length);
            if (!InfoQuantity::count(tmp)) continue;
            for (int anpos = pos + length - 1, anlen; anpos > pos; --anpos)
            {
                for (anlen = 7; anpos + anlen > pos + length && anpos < (int)sentence.size(); --anlen)
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
        pos = ambiguity.count(pos) ? ambiguity[pos] + 1 : (pos + 1);
    }

    return ambiguity;
}

// calculate the order of given section name as '1.5.2 <NAME>' and return the result
int cal_order_by_secname(const std::string& secname)
{
    int first, second, third; char tmp;
    std::stringstream ss(secname);
    ss >> first >> tmp >> second >> tmp >> third;
    return first * 10000 + second * 100 + third;
}
} // namespace

// contructor of FileInfoWithAllKeywords
FileInfoWithAllKeywords::FileInfoWithAllKeywords(const std::string& filepath) : filepath(filepath) {}

std::map<std::string, double> InvertedIndex::cal_scores_(const data_type& kw_infos_mapping)
{
    std::map<std::string, double> scores; // init scores

    for (auto& kw_infos : kw_infos_mapping)
    {
        auto& infos = kw_infos.second;
        for (int i = 0, concept_pos = -1; i < (int)infos.size(); ++i)
        {
            if (infos[i].is_appeared_in_title)
            {
                scores[infos[i].filepath] += 1000;
                concept_pos = filesorder_[infos[i].filepath];
            }
            else if (concept_pos > -1)
                scores[infos[i].filepath] -= abs(filesorder_[infos[i].filepath] - concept_pos) * 100;

            scores[infos[i].filepath] += infos[i].times * InfoQuantity::get_infoquantity(kw_infos.first);
        }
    }

    return scores;
}

InvertedIndex::InvertedIndex(const std::string & filepath) : tempfilepath_(filepath)
{
    ready_ = unserialize();
#ifdef _DEBUG
    printf("[InvertedIndex] [Unserialize] [Path] [%s] [Result] [%s]\n", filepath.c_str(), ready_ ? "true" : "false");
#endif
}

bool InvertedIndex::ready() const
{
    return ready_;
}

/*
Structure of temp file for serialization
*/

bool InvertedIndex::serialize()
{
    auto fout = std::ofstream(tempfilepath_);

    if (!fout) return false;

    for (auto& kw_infos : kw_infos_mapping_)
    {
        fout << kw_infos.first << std::endl;
        for (auto& infos : kw_infos.second)
        {
            fout << infos.filepath << std::endl;
            fout << infos.times << ' ' << infos.density << ' ' << infos.is_appeared_in_title << std::endl;
        }
        fout << std::endl;
    }

    return true;
}

bool InvertedIndex::unserialize()
{
    std::ifstream fin(tempfilepath_);
    if (!fin) return false;

    std::string line, kw;
    while (std::getline(fin, kw))
    {
        while (std::getline(fin, line))
        {
            if (line.empty()) break;

            double times, density;
            bool is_appeared_in_title;

            fin >> times >> density >> is_appeared_in_title; char c; fin.get(c);
            kw_infos_mapping_[kw].push_back({ line, times, density, is_appeared_in_title });
        }
    }

    return !kw_infos_mapping_.empty();
}

std::vector<FileInfoWithAllKeywords> InvertedIndex::get_fileinfos(const std::vector<key_type> & keywords, int pagenum, int perpage)
{
    data_type kw_infos_mapping, for_cal_scores;
    std::vector<std::vector<std::string>> paths_list;
    for (auto& kw : keywords)
    {
        kw_infos_mapping[kw] = kw_infos_mapping_[kw];
        paths_list.push_back(std::vector<std::string>());
        for (auto& info : kw_infos_mapping[kw]) paths_list.back().push_back(info.filepath);
    }

    std::map<std::string, FileInfoWithAllKeywords> path_infos_mapping;
    for (const auto &filepath : combine(paths_list)) path_infos_mapping.emplace(filepath, filepath);

    for (auto& path_infos : path_infos_mapping) for (auto& kw_infos : kw_infos_mapping) for (auto& info : kw_infos.second)
        if (info.filepath == path_infos.first)
    {
        for_cal_scores[kw_infos.first].push_back(info);
        path_infos.second.kwinfos[kw_infos.first] =
        {
            info.times, info.density, info.is_appeared_in_title
        };
    }

    std::vector<FileInfoWithAllKeywords> fileinfos;
    for (auto& path_infos : path_infos_mapping)
    {
        fileinfos.push_back(path_infos.second);
    }

    auto scores = cal_scores_(for_cal_scores);
    std::sort(fileinfos.rbegin(), fileinfos.rend(), [&](const FileInfoWithAllKeywords & a, const FileInfoWithAllKeywords & b)
        {
            return scores[a.filepath] < scores[b.filepath];
        });

    return decltype(fileinfos)(fileinfos.begin() + pagenum * perpage - perpage, fileinfos.begin() + pagenum * perpage);
}

std::vector<std::string> InvertedIndex::get_filepaths(const std::string &keywords)
{
    return get_filepaths(Segmentation::segment(keywords));
}

std::vector<std::string> InvertedIndex::get_filepaths(const std::vector<key_type> & keywords)
{
#ifdef _DEBUG
    printf("[InvertedIndex] [Getfilepaths]\n");
#endif
    data_type kw_infos_mapping, for_cal_scores;
    std::vector<std::vector<std::string>> paths_list;
    for (auto& kw : keywords)
    {
        kw_infos_mapping[kw] = kw_infos_mapping_[kw];
        paths_list.push_back(std::vector<std::string>());
        for (auto& info : kw_infos_mapping[kw]) paths_list.back().push_back(info.filepath);
    }
#ifdef _DEBUG
    printf("[InvertedIndex] [Getfilepaths] [Combine]\n");
#endif
    auto filepaths = combine(paths_list);
#ifdef _DEBUG
    printf("[InvertedIndex] [Getfilepaths] [Combine] [Over]\n");
#endif
    for (auto& filepath : filepaths) for (auto& kw_infos : kw_infos_mapping) for (auto& info : kw_infos.second)
        if (info.filepath == filepath) for_cal_scores[kw_infos.first].push_back(info);

    auto scores = cal_scores_(for_cal_scores);
    std::sort(filepaths.rbegin(), filepaths.rend(), [&](const std::string & a, const std::string & b)
        {
            return scores[a] < scores[b];
        });
#ifdef _DEBUG
    printf("[InvertedIndex] [Getfilepaths] [Over]\n");
#endif
    return filepaths;
}

void InvertedIndex::add_files(const std::string & folderpath)
{
#ifdef _DEBUG
    printf("[InvertedIndex] [FolerPath] [%s]\n", folderpath.c_str());
#endif

    std::vector<fs::path> paths;

    for (auto &p : fs::directory_iterator(folderpath))
    {
        if (p.is_regular_file()) paths.push_back(p.path());
    }

#ifdef _DEBUG
    printf("[InvertedIndex] [Numofpaths] [%d]\n", (int)paths.size());
#endif

    sort(paths.begin(), paths.end(), [&](const fs::path & a, const fs::path & b)
        {
            return cal_order_by_secname(a.filename().string()) < cal_order_by_secname(b.filename().string());
        });

    for (auto &path : paths)
    {
        std::ifstream fin(path);
        auto content = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());
        add_file(content, path.string());
        filesorder_[path.string()] = filesorder_.size();
    }

    serialize();
}

void InvertedIndex::add_file(const key_type & sentence, const std::string & filepath)
{
#ifdef _DEBUG
    printf("[InvertedIndex] [Addfile] [Filepath] [%s]\n", filepath.c_str());
#endif
    double alltimes = 0.0;

    std::map<key_type, _KeywordInfo> kwinfos;

    auto ambiguities = get_ambiguity_section(sentence);
    for (auto& mp : ambiguities)
    {
        if (sentence[mp.first] == '$')
        {
            auto formula = sentence.substr(mp.first, mp.second - mp.first).raw();
            for (const auto &formula : Segmentation::get_all_formulas(formula))
            {
                alltimes += 1;
                kwinfos["$" + formula].times += 1;
            }
        }
        else for (auto& word : Segmentation::segment(sentence.substr(mp.first, mp.second - mp.first)))
        {
            alltimes += 1;
            kwinfos[word].times += 1;
            kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
        }
    }

    int endpos = -1;
    for (auto& mp : ambiguities)
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

    for (auto& kwinfo : kwinfos)
    {
        auto& info = kwinfo.second;
        kw_infos_mapping_[kwinfo.first].push_back({ filepath, info.times, info.times / alltimes, info.is_appeared_in_title });
    }
}
