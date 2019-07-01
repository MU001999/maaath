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

// calculate the order of given section name as '1.5.2 <NAME>' and return the result
int cal_order_by_secname(const std::string& secname)
{
    int first, second, third; char tmp;
    std::stringstream ss(secname);
    ss >> first >> tmp >> second >> tmp >> third;
    return first * 10000 + second * 100 + third;
}
} // namespace

std::map<std::string, double> InvertedIndex::cal_scores_(const data_type& kw_infos_mapping)
{
    std::map<std::string, double> scores; // init scores

    for (auto& kw_infos : kw_infos_mapping)
    {
        int i, concept_pos = -1;

        auto& infos = kw_infos.second;
        for (i = 0; i < (int)infos.size(); ++i)
        {
            if (infos[i].is_appeared_in_title)
            {
                scores[infos[i].filepath] += 1000;
                concept_pos = filesorder_[infos[i].filepath];
            }
            scores[infos[i].filepath] += infos[i].times * InfoQuantity::get_infoquantity(kw_infos.first);
        }

        if (concept_pos != -1) for (i = 0; i < (int)infos.size(); ++i)
        {
            scores[infos[i].filepath] -= abs(filesorder_[infos[i].filepath] - concept_pos) * 100;
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

    for (auto &word : Segmentation::segment(sentence))
    {
        alltimes += 1;
        kwinfos[word].times += 1;
        kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
    }

    for (auto &kwinfo : kwinfos)
    {
        auto &info = kwinfo.second;
        kw_infos_mapping_[kwinfo.first].push_back({ filepath, info.times, info.times / alltimes, info.is_appeared_in_title });
    }
}
