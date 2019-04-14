#pragma once


class Server
{
private:
    int listen_fd_;

public:
    
    Server();

    ~Server();

    void listen();

    void run();
};
