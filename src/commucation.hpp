#pragma once

#include <string>


/*
Request:
[TYPE_ID][KEYWORDS]

Response4search:
filename#...#
path#...#
abstract#...#
...

Response4knowgraph:
keywordpair#...# (first pair of concepts, delim is '-', for example, CONCEPT1-CONCEPT2)
keywordpair#...# (second ...)
...
keywordnum#...# (count of first concept, delim is '-', for example, CONCEPT-10)
keywordnum#...# (count of second ...)
...
*/


class Request
{
public:
    enum CommType
    {
        Article = 0,
        Definition,
        Property,
        Example,
        Exercise,
        ConceptMap
    };

    Request(const std::string &raw);
    ~Request() = default;

    CommType type() const;
    std::string keywords() const;

private:
    std::string keywords_;
    CommType type_;
};
