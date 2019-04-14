#include "search_server.hpp"


int main(int argc, char *argv[])
{
    SearchServer ss;
    ss.listen();
    ss.run();

    return 0;
}