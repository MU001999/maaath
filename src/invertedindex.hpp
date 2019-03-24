#pragma once


class InvertedIndex
{
public:
    using key_type = Utf8String;
    using value_type = std::vector<int>;

private:
    static std::map<key_type, value_type> files; // Each word id in the index corresponds to a list of file ids

public:
    static value_type get_files_list(const key_type &sentence);
    static void add_file(const key_type &sentence, int file_id); // utf8_string word
};
