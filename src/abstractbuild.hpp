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
    double score;
};


class AbstractBuilder {
private:
    std::vector<std::string> keywords_;
    std::string abstract_;

    std::string read_file_(const std::string &filepath);
    std::vector<std::string> sentence_filter_(const std::string &content);
    std::priority_queue<Sentence> divide_sentence_(const Utf8String &article, const Utf8String &pattern = "，");
    double score_sentence_(const std::string& sentence);
    void get_abstract_(const std::vector<std::string>& sentences);
    void parse_file_(const std::string& filepath);

public:
    AbstractBuilder(const std::vector<std::string> &keywords, const std::string &filepath);
    ~AbstractBuilder() = default;

    std::string abstract();

    static std::string gen_abstract(const std::vector<std::string> &keywords, const std::string &filepaths);
};