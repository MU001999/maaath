#pragma once


class SearchServer
{
private:
    int listen_fd_;

public:
    
    SearchServer();

    ~SearchServer();

    void listen();

    void run();
};//
