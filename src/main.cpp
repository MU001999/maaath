#include <array>
#include <string>

#include <unistd.h>

#include "invertedindex.hpp"
#include "search_server.hpp"

// #define _DEBUG


// global inverted index objects
std::array<InvertedIndex, 5> iis = {
    InvertedIndex("../tmp/article.tmp"),
    InvertedIndex("../tmp/definition.tmp"),
    InvertedIndex("../tmp/property.tmp"),
    InvertedIndex("../tmp/example.tmp"),
    InvertedIndex("../tmp/exercise.tmp"),
};

// static folder paths for inverted index objects
static std::array<std::string, 5> _folderpaths = {
    "../resources/article",
    "../resources/definition",
    "../resources/property",
    "../resources/example",
    "../resources/exercise"
};

int main(int argc, char *argv[])
{
#ifndef _DEBUG
    // change program to daemon
    daemon(0, 0);
#endif

    for (std::size_t i = 0; i < 5; ++i)
    {
        // init if unserialize failed
        if (!iis[i].ready()) iis[i].add_files(_folderpaths[i]);
    }

    // declare server
    Server server;
    server.listen();
    server.run();

    return 0;
}