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

    
    static bool serialize();

    static bool unserialize();

    static value_type get_files_list(const key_type &sentence);
    
    // Add files with receiving a folder path
    static void add_files(const std::string &folderpath);

    static void add_file(const key_type &sentence, const std::string &filepath);
};
