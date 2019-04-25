#include <string>
#include <fstream>
#include <iostream>

#include "utf8string.hpp"
#include "abstractbuild.hpp"

// #define _DEBUG
#ifdef _DEBUG
#include <cstdio>
#include <iostream>
#endif


static constexpr auto _kMaxSentence   = 128,
                      _kDensityWeight = 2000,
                      _kAbstractNum   = 10;


bool operator<(const Sentence &a, const Sentence &b)
{
    return a.score > b.score;
}

std::string AbstractBuilder::read_file_(const std::string& filepath)
{
#ifdef _DEBUG
    printf("[AbstractBuild] [Readfile]\n");
#endif
    std::ifstream fin(filepath);
    if (!fin) return "";
    std::string content, line;
    for (; std::getline(fin, line); content += line + " ");
    return content;
}

std::vector<std::string> AbstractBuilder::sentence_filter_(const std::string &article)
{
    std::vector<std::string> allsentences;

    auto sentences = divide_sentence_(article, '.');
    while (!sentences.empty()) {
        auto stc = sentences.top();
        if (allsentences.empty() || allsentences.back() != stc.content)
            allsentences.push_back(stc.content);
        sentences.pop();
    }
#ifdef _DEBUG
    printf("[AbstractBuild] [SentenceFilter] [Over]\n");
#endif
    return allsentences;
}

std::priority_queue<Sentence> AbstractBuilder::divide_sentence_(const Utf8String &article, char pattern)
{
    std::priority_queue<Sentence> sentences;

    std::size_t start = 0, pos = article.find(pattern);
    while (pos != article.npos) 
    {
        if (start != pos) 
        {
            auto content = article.substr(start, pos - start);
            sentences.push({ content.raw(), score_sentence_(content.raw()) });
        }
        start = pos + 1;
        pos = article.find(pattern, start);
    }
    if (start < article.size()) 
    {
        auto content = article.substr(start);
        sentences.push({ content.raw(), score_sentence_(content.raw()) });
    }
#ifdef _DEBUG
    printf("[AbstractBuild] [DivideSentence] [Over]\n");
#endif
    return sentences;
}

void AbstractBuilder::parse_file_(const std::string &filepath)
{
#ifdef _DEBUG
    printf("[AbstractBuild] [Parsefile]\n");
#endif
    get_abstract_(sentence_filter_(read_file_(filepath)));
}

double AbstractBuilder::score_sentence_(const std::string &sentence)
{
#ifdef _DEBUG
    printf("[AbstractBuild] [ScoreSentence]\n");
#endif
    if (sentence.empty()) return 0;

    double numerator = 0, denominator = sentence.size();
    for (const auto &keyword : keywords_)
    {
        if (keyword.front() == '$')
        {
            // TODO: te shu chu li
        }
        else
        {
            std::size_t start = 0, pos = sentence.find(keyword);
            while (pos != std::string::npos) {
                if (start != pos) numerator += keywords_.size();
                start = pos + 1;
                pos = sentence.find(keyword, start);
            }
        }
    }

    if (numerator == 0) return 0;
    else return numerator / denominator * _kDensityWeight + (double)(sentence.size());
}

void AbstractBuilder::get_abstract_(const std::vector<std::string> &sentences)
{
#ifdef _DEBUG
    printf("[AbstractBuild] [GetAbstract]\n");
#endif
    auto it = sentences.end();
    for (std::size_t i = 0; i < _kAbstractNum && it > sentences.begin(); ++i)
        abstract_ += *--it + "。";
}

AbstractBuilder::AbstractBuilder(const std::vector<std::string> &keywords, const std::string &filepath) : keywords_(keywords)
{
#ifdef _DEBUG
    printf("[AbstractBuild] [AbstractBuilder]\n");
#endif
    parse_file_(filepath);
}

std::string AbstractBuilder::abstract()
{
    return abstract_;
}

std::string AbstractBuilder::gen_abstract(const std::vector<std::string> &keywords, const std::string &filepath)
{
#ifdef _DEBUG
    printf("[AbstractBuild]\n");
#endif
    return AbstractBuilder(keywords, filepath).abstract();
}