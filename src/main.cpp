#include "search_server.hpp"


int main(int argc, char *argv[])
{
    Server server;
    server.listen();
    server.run();

    return 0;
}