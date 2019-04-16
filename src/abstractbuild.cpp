#include <string>
#include <fstream>
#include <iostream>

#include "utf8string.hpp"
#include "abstractbuild.hpp"

static constexpr auto _kMaxSentence   = 128,
                      _kDensityWeight = 2000,
                      _kAbstractNum   = 5;
static Utf8String _patterns = "。，；";


bool operator<(const Sentence &a, const Sentence &b)
{
    return a.score > b.score;
}

std::string AbstractBuilder::read_file_(const std::string& filepath)
{
    std::ifstream fin(filepath);
    std::string content, line;
    for (; std::getline(fin, line); content += line + " ");
    return content;
}

AbstractBuilder::StrList AbstractBuilder::sentence_filter_(const std::string &article)
{
    StrList allsentences;

    auto sentences = divide_sentence_(article, _patterns.front());
    while (!sentences.empty()) {
        Sentence stc = sentences.top();
        if (allsentences.back() != stc.content)
            allsentences.push_back(stc.content);
        sentences.pop();
    }

    return allsentences;
}

AbstractBuilder::StrPriQueue AbstractBuilder::divide_sentence_(const Utf8String &article, const Utf8String &pattern)
{
    StrPriQueue sentences;

    std::size_t start = 0, pos = article.find(pattern);
    while (pos != std::string::npos) {
        if (start != pos) {
            auto content = article.substr(start, pos - start);
            sentences.push({ content.raw(), score_sentence_(content.raw()) });
        }
        start = pos + 1;
        pos = article.find(pattern, start);
    }
    if (start < article.size()) {
        auto content = article.substr(start);
        sentences.push({content.raw(), score_sentence_(content.raw()) });
    }

    return sentences;
}

void AbstractBuilder::parse_file_(const std::string &filepath)
{
    get_abstract_(sentence_filter_(read_file_(filepath)));
}

double AbstractBuilder::score_sentence_(const std::string &sentence)
{
    if (sentence.empty()) return 0;

    double numerator = 0, denominator = sentence.size();
    for (const auto &keyword : keywords_) {
        std::size_t start = 0, pos = sentence.find(keyword);
        while (pos != std::string::npos) {
            if (start != pos) numerator += keywords_.size();
            start = pos + 1;
            pos = sentence.find(keyword, start);
        }
    }

    if (numerator == 0) return 0;
    else return numerator / denominator * _kDensityWeight + (double)(sentence.size());
}

void AbstractBuilder::get_abstract_(const StrList &sentences)
{
    auto it = sentences.end();
    for (std::size_t i = 0; i < _kAbstractNum && it > sentences.begin(); ++i)
        abstract_ += *--it + "。";
}

AbstractBuilder::AbstractBuilder(const StrList &keywords, const std::string &filepath) : keywords_(keywords)
{
    parse_file_(filepath);
}

std::string AbstractBuilder::abstract()
{
    return abstract_;
}

std::string AbstractBuilder::gen_abstract(const std::vector<std::string> &keywords, const std::string &filepath)
{
    return AbstractBuilder(keywords, filepath).abstract();
}