#pragma once


struct FileInfo
{
    std::string filepath;
    double times = 0.0, density = 0.0;
    bool is_appeared_in_title = false;
};


class InvertedIndex
{
public:
    using key_type = Utf8String;
    using value_type = std::vector<FileInfo>;
    using data_type = std::map<key_type, value_type>;

private:
    data_type files;
    std::string tempfilepath;

    std::map<std::string, double> cal_scores(const data_type &data);

public:
    InvertedIndex(const std::string &filepath);

    bool serialize();

    bool unserialize();

    value_type get_fileinfos(const key_type &sentence);

    std::vector<std::string> get_filepaths(const std::vector<key_type> &keywords);

    // Add files with receiving a folder path
    void add_files(const std::string &folderpath = "./inputfiles");

    void add_file(const key_type &sentence, const std::string &filepath);
};
