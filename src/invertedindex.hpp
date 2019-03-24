#pragma once


class InvertedIndex
{
public:
    using key_type = Utf8String;
    using value_type = std::vector<int>;

private:
    static std::map<key_type, value_type> files; // Each word id in the index corresponds to a list of file ids

public:
    static value_type get_files_list(const key_type &utf8_string);
    static void add_file(const key_type &utf8_string, int file_id); // utf8_string word
};
