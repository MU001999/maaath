#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/wait.h>
#include<stddef.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/select.h>
#include<unistd.h>


#define LISTENNUM 1024
#define UNPATH "datastruct.sock"
#define THREADNUM 1024
class SearchServer
{
    private:
        int listenfd;
    public:
    
        SearchServer();

        ~SearchServer();

        void return_search();//use thread to send msg to webserver
};//
