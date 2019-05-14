#include <array>
#include <string>

#include <unistd.h>

#include "searchserver.hpp"
#include "invertedindex.hpp"

// #define _DEBUG
#ifdef _DEBUG
#include <cstdio>
#include <iostream>
#endif


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
    "../resources/Document/Article",
    "../resources/Document/Definition",
    "../resources/Document/Property",
    "../resources/Document/Example",
    "../resources/Document/Exercise"
};

int main(int argc, char *argv[])
{
#ifndef _DEBUG
    // change program to daemon
    daemon(1, 0);
#endif

    for (int i = 0; i < 5; ++i)
    {
        // init if unserialize failed
        if (!iis[i].ready())
        {
#ifdef _DEBUG
            printf("[Main] [IIS] [Addfiles] [%d] [Filepath] [%s]\n", i, _folderpaths[i].c_str());
#endif
            iis[i].add_files(_folderpaths[i]);
        }
    }

    // declare server
    Server server;
    server.listen();
    server.run();

    return 0;
}