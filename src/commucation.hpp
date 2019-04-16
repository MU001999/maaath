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
keyword#...# (first keyword after segmentation)
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
