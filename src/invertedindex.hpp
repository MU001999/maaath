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
<<<<<<< HEAD
    static std::map<std::string, double> cal_scores(const data_type &files,info_quantity &freq);
=======
    static std::map<std::string, double> cal_scores(const data_type &data);
>>>>>>> 9d13db8b4d6e4c6db0eb02176829cf1d7b171b2f

public:
    static bool serialize(const std::string &filepath = "./dseii.tmp");

    static bool unserialize(const std::string &fllepath = "./dseii.tmp");

    static value_type get_fileinfos(const key_type &sentence);

    static std::vector<std::string> get_filepaths(const std::vector<key_type> &keywords);

    // Add files with receiving a folder path
    static void add_files(const std::string &folderpath = "./inputfiles");

    static void add_file(const key_type &sentence, const std::string &filepath);
};
