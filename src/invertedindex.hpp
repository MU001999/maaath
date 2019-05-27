#pragma once

#include <map>
#include <string>
#include <vector>
#include <unordered_map>

#include "utf8string.hpp"

// struct contains filepath and one keyword's info in it
struct FileInfo
{
    std::string filepath;
    double times = 0.0, density = 0.0;
    bool is_appeared_in_title = false;
};

// struct contains single keyword's info in one file
struct KeywordInfo
{
    double times = 0.0, density = 0.0;
    bool is_appeared_in_title = false;
};

// struct contains filepath and all keywords' info in it
struct FileInfoWithAllKeywords
{
    std::string filepath;
    std::map<Utf8String, KeywordInfo> kwinfos;

    FileInfoWithAllKeywords(const std::string& filepath);
};

class InvertedIndex
{
public:
    using key_type = Utf8String;
    using value_type = std::vector<FileInfo>;
    using data_type = std::map<key_type, value_type>;

private:
    bool ready_;
    std::string tempfilepath_;
    data_type kw_infos_mapping_;
    std::unordered_map<std::string, int> filesorder_;

    // calculate scores of given files
    std::map<std::string, double> cal_scores_(const data_type& kw_infos_mapping);

public:
    // constructor, recerives the serialization file
    InvertedIndex(const std::string& filepath);

    // returns ready or not
    bool ready() const;

    // serialize to serialization file
    bool serialize();

    // unserialize from serialization file
    bool unserialize();

    // returns fileinfos by given keywords
    std::vector<FileInfoWithAllKeywords> get_fileinfos(const std::vector<key_type>& keywords, int pagenum = 1, int perpage = 10);

    std::vector<std::string> get_filepaths(const std::string &keywords);

    // returns filepaths by given keywords
    std::vector<std::string> get_filepaths(const std::vector<key_type>& keywords);

    // add files with receiving a folder path
    void add_files(const std::string& folderpath);

    // add all the words in a sentence to the inverted index
    void add_file(const key_type& sentence, const std::string& filepath);
};
