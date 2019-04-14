#include <cstring>
#include <cstdlib>

#include <thread>
#include <string>
#include <functional>

#include <fcntl.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#include"utf8string.hpp"
#include"commucation.hpp"
#include"search_server.hpp"

#define UN_PATH "/tmp/datastructureexpt.socket"


static void _process(int fd, sockaddr_un un, socklen_t len)
{
    char buff[4096];
    if (read(fd, buff, 4096) == -1) return;
    
    std::string result;
    // get result

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