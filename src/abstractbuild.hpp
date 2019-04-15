#pragma once

#include <list>
#include <queue>
#include <string>
#include <vector>
#include <iterator>


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
    std::string filepath_;
    StrList abstracts_;

    static std::string read_file_(const std::string &filepath);
    static StrList sentence_filter_(const std::string &content);
    
public:
    AbstractBuilder(const StrList &keywords, const std::string &filepaths);
    ~AbstractBuilder() = default;

    void parse_file(const std::string &filepath);


    StrPriQueue divide_sentence(std::string, char);

    float score_sentence(std::string);

    void check_sentence(StrList);

    void get_abstract(StrList);

    std::string gen_abstract();

    static std::string gen_abstract(const std::vector<std::string> &keywords, const std::string &filepaths);
};