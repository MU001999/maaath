#pragma once

class InvertedIndex
{
private:
    std::map<Utf8String, std::set<int>> index; // Each word id in the index corresponds to a list of file ids

public:
    InvertedIndex();
    std::set<int> get_file_list(const Utf8String &utf8_string);
    void add_file(Utf8String utf8_string, int file_id, InfoQuantity dict); // utf8_string word
};
