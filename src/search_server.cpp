#include"search_server.hpp"
#include"utf8string.hpp"
#include"commucation.cpp"
struct thread_info
{
    pthread_t conn;
    int flag;
    int connfd;
};
SearchServer::SearchServer()
{
    int tempfd,offset;
    struct sockaddr_un un;
    tempfd = socket(AF_UNIX,SOCK_STREAM,0);
    if(tempfd<0)
        exit(-2);
    this->listenfd = tempfd;
    const char * path = UNPATH;
    unlink(path);
    memset(un,0,sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path,path);
    offset = offsetof(struct sockaddr_un,sun_path);
    bind(this->listenfd,(struct sockaddr*)&un,offset);
    listen(this->listenfd,LISTENNUM);
    int flag = fcntl(this->listenfd,F_GETFL,0);
    fcntl(this->listenfd,F_SETFL,flag|O_NONBLOCK);
}
SearchServer::~SearchServer()
{
    shutdown(this->listenfd,2);
}
void * send_response(void * connfd)
{
    char* request[4096];
    char* response;
    int response_len;
    struct thread_info* ptr = (struct thread_info*)connfd;
    while(1)
    {
        int n = recv(ptr->connfd,request,BUFFSIZE,0);
        /* get search request*/
        int flags = send(ptr->connfd,response,response_len,0);
        /*get errno*/
    }
    ptr->flag = -1;
    ptr->connfd = -1;
}
void SearchServer::return_search()
{
    struct thread_info pool[THREADNUM];
    for(int i = 0;i<THREADNUM;i++)
        pool.flag = -1;
    int connfd;
    struct sockaddr_in client;
    socklen_t connlen = sizeof(client);
    while(1)
    {
        connfd = accept(this->listenfd,(struct sockaddr*)&client,&connlen);
        if(connfd==-1)
        {
            if(errno==EAGAIN)continue;
            else break;
        }
        else
        {   int i;
            for(i = 0;i<THREADNUM;i++)
            {
                if(pool.flag==-1)
                {   
                    pool[i].connfd = connfd;
                    pthread_create(&pool[i].conn,NULL,send_response,&pool[i]);
                }
            }
            if(i==THREADNUM)
            {
                send(connfd,"the pool is full\n",20,0);
            }
        }
    }
}
