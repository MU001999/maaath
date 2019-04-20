#pragma once


class Server
{
private:
    int listen_fd_;

public:
    Server();
    ~Server();

    // to listen
    void listen();

    // to accept and process
    void run();
};
