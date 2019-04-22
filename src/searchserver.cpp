#include <cstring>
#include <cstdlib>

#include <array>
#include <vector>
#include <thread>
#include <string>
#include <utility>
#include <functional>

#include <fcntl.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "utf8string.hpp"
#include "commucation.hpp"
#include "segmentation.hpp"
#include "searchserver.hpp"
#include "invertedindex.hpp"
#include "abstractbuild.hpp"

#define UN_PATH "/tmp/datastructureexpt.socket"

#define _DEBUG
#ifdef _DEBUG
#include <cstdio>
#include <iostream>
#endif


extern std::array<InvertedIndex, 5> iis;

static std::string _gen_response(const std::vector<std::pair<std::string, std::string>> &pairs)
{
    std::string result;
    for (const auto &p : pairs) result += p.first + "#" + p.second + "#";
    return result;
}

static void _process(int fd, sockaddr_un un, socklen_t len)
{
    char buff[4096] = {0};
    if (read(fd, buff, 4096) == -1) return;

    std::vector<std::pair<std::string, std::string>> pairs;
    Request req(buff);
    auto keywords = req.keywords().front() == '$'
        // ? std::vector<Utf8String>{ get_all_formulas(req.keywords().substr(0, req.keywords().find('$', 1))).front() };
        ? std::vector<Utf8String>{}
        : Segmentation::segment(req.keywords());
#ifdef _DEBUG
    printf("[Server] [Receive] [Type %d] [Keywords] [%s]\n", req.type(), req.keywords().c_str());
    printf("[Server] [Keywords] [Segment] [Result] [ ");
    for (auto& keyword : keywords) printf("%s ", keyword.c_str());
    printf("]\n");
#endif
    if (req.type() == Request::ConceptMap) for (auto &keyword : keywords)
    {
        pairs.push_back({"keyword", keyword.raw()});
    }
    else
    {
        std::vector<std::string> kws;
        for (auto& kw : keywords) kws.push_back(kw.raw());
        auto filepaths = iis[req.type()].get_filepaths(keywords);
#ifdef _DEBUG
        printf("[Server] [Getfilepaths] [Over]\n");
#endif
        for (const auto& filepath : filepaths)
        {
#ifdef _DEBUG
            printf("[Server] [Processing] [Filepath] [%s]\n", filepath.c_str());
#endif
            pairs.push_back({ "filename", filepath.substr(filepath.rfind('/') + 1) });
            pairs.push_back({ "path", filepath });
            pairs.push_back({ "abstract", AbstractBuilder::gen_abstract(kws, filepath) });
#ifdef _DEBUG
            printf("[Server] [Processing] [Filepath] [%s] [Over]\n", filepath.c_str());
#endif
        }
    }
    
    auto result = _gen_response(pairs);
    if (write(fd, result.c_str(), result.size() + 1) == -1) return;

    close(fd);
}


Server::Server() : listen_fd_(socket(AF_UNIX, SOCK_STREAM, 0))
{
#ifdef _DEBUG
    printf("[Server] [Constructor]\n");
#endif
    if (listen_fd_ == -1) abort();
    unlink(UN_PATH);

    sockaddr_un un;
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, UN_PATH);

    auto size = offsetof(sockaddr_un, sun_path) + strlen(un.sun_path);
    if (bind(listen_fd_, (sockaddr *)&un, size) < 0) abort();
#ifdef _DEBUG
    printf("[Server] [Constructor] [Over]\n");
#endif
}

Server::~Server()
{
    close(listen_fd_);
}

void Server::listen()
{
    fcntl(listen_fd_, F_SETFL, fcntl(listen_fd_, F_GETFL, 0) | O_NONBLOCK);
    ::listen(listen_fd_, 1024);
#ifdef _DEBUG
    printf("[Server] [Listen] [Over]\n");
#endif
}

void Server::run()
{
    sockaddr_un remote_addr;
    socklen_t len = sizeof(remote_addr);
#ifdef _DEBUG
    printf("[Server] [Run] [Ready]\n");
#endif
    while (true)
    {
        auto client_fd = accept(listen_fd_, (sockaddr *)&remote_addr, &len);
        if (client_fd == -1) continue;
#ifdef _DEBUG
        printf("[Server] [Run] [Loop] [Accept] [Fd] [%d]\n", client_fd);
#endif
        std::thread t(std::bind(_process, client_fd, remote_addr, len));
        t.detach();
    }
}