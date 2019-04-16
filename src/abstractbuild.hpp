#pragma once

#include <list>
#include <queue>
#include <string>
#include <vector>
#include <iterator>

class Utf8String;


struct Sentence
{
    std::string content;
    float score;
};


class AbstractBuilder {
private:
    using StrList = std::vector<std::string>;
    using StrPriQueue = std::priority_queue<Sentence>;
    using StrListIter = StrList::iterator;

    StrList keywords_;
    std::string abstract_;

    std::string read_file_(const std::string &filepath);
    StrList sentence_filter_(const std::string &content);
    StrPriQueue divide_sentence_(const Utf8String &article, const Utf8String &pattern);
    double score_sentence_(const std::string& sentence);
    void get_abstract_(const StrList& sentences);
    void parse_file_(const std::string& filepath);

public:
    AbstractBuilder(const StrList &keywords, const std::string &filepath);
    ~AbstractBuilder() = default;

    std::string abstract();

    static std::string gen_abstract(const std::vector<std::string> &keywords, const std::string &filepaths);
};