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
#include "search_server.hpp"
#include "invertedindex.hpp"
#include "abstractbuild.hpp"

#define UN_PATH "/tmp/datastructureexpt.socket"


extern std::array<InvertedIndex, 5> iis;

static std::string _gen_response(const std::vector<std::pair<std::string, std::string>> &pairs)
{
    std::string result;
    for (const auto &p : pairs) result += p.first + "#" + p.second + "#";
    return result;
}

static void _process(int fd, sockaddr_un un, socklen_t len)
{
    char buff[4096];
    if (read(fd, buff, 4096) == -1) return;

    std::string result;

    Request req(buff);
    auto keywords = Segmentation::segment(req.keywords());
    if (req.type() == Request::ConceptMap)
    {
        // gen conceptmap
    }
    else
    {
        std::vector<std::pair<std::string, std::string>> pairs;
        std::vector<std::string> kws;
        for (const auto &kw : keywords) kws.push_back(kw.raw());
        auto filepaths = iis[req.type()].get_filepaths(keywords);
        for (const auto &filepath : filepaths)
        {
            pairs.push_back({"filename", filepath.substr(filepath.rfind('/')+1)});
            pairs.push_back({"path", filepath});
            pairs.push_back({"abstract", AbstractBuilder::gen_abstract(kws, filepaths)});
        }
        result = _gen_response(pairs);
    }

    if (write(fd, result.c_str(), result.size() + 1) == -1) return;

    close(fd);
}


Server::Server() : listen_fd_(socket(AF_UNIX, SOCK_STREAM, 0))
{
    if (listen_fd_ == -1) abort();
    unlink(UN_PATH);

    sockaddr_un un;
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, UN_PATH);

    auto size = offsetof(sockaddr_un, sun_path) + strlen(un.sun_path);
    if (bind(listen_fd_, (sockaddr *)&un, size) < 0) abort();
}

Server::~Server()
{
    close(listen_fd_);
}

void Server::listen()
{
    fcntl(listen_fd_, F_SETFL, fcntl(listen_fd_, F_GETFL, 0) | O_NONBLOCK);
    ::listen(listen_fd_, 1024);
}

void Server::run()
{
    sockaddr_un remote_addr;
    socklen_t len = sizeof(remote_addr);

    while (true)
    {
        auto client_fd = accept(listen_fd_, (sockaddr *)&remote_addr, &len);
        if (client_fd == -1) continue;

        std::thread t(std::bind(_process, client_fd, remote_addr, len));
        t.detach();
    }
}