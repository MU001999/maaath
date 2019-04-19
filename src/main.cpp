#include <array>
#include <string>

#include "invertedindex.hpp"
#include "search_server.hpp"


std::array<InvertedIndex, 5> iis = {
    InvertedIndex("../tmp/article.tmp"),
    InvertedIndex("../tmp/definition.tmp"),
    InvertedIndex("../tmp/property.tmp"),
    InvertedIndex("../tmp/example.tmp"),
    InvertedIndex("../tmp/exercise.tmp"),
};

static std::array<std::string, 5> _folderpaths = {
    "../resources/article",
    "../resources/definition",
    "../resources/property",
    "../resources/example",
    "../resources/exercise"
};

int main(int argc, char *argv[])
{
    for (std::size_t i = 0; i < 5; ++i)
    {
        if (!iis[i].ready()) iis[i].add_files(_folderpaths[i]);
    }

    Server server;
    server.listen();
    server.run();

    return 0;
}