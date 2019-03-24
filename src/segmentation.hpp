#pragma once


struct Wordmap
{
    Utf8String word;
    int pos_in_sentence;
};


struct Segmentation
{
    static std::vector<Wordmap> segment(const Utf8String &sentence);
};
