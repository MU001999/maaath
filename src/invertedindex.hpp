#pragma once


struct wordmap
{
    Utf8String word;
    int pos_in_sentence;
};


struct Segmentation
{
    static std::vector<wordmap> segment(const Utf8String &sentence);
};


class InvertedIndex
{
private:
    std::map<Utf8String, std::set<int>> index; // Each word id in the index corresponds to a list of file ids

public:
    InvertedIndex();
    ~InvertedIndex() {}

    std::set<int> get_file_list(const Utf8String &utf8_string);
    void add_file(const Utf8String &utf8_string, int file_id); // utf8_string word
};
