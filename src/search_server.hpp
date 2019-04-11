#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/wait.h>

#define LISTNUM 1024
class SearchServer{
    private:
        int listenfd;
        void * connect_thread(void * connfd);
    public:
        SearchServer();

        ~SearchServer();

        void return_search(int listenfd);//use thread to send msg to webserver
};//