#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#define RECV_BUF_SIZE 100
#define MAX_EVENTS 50
int mIndex=-1;
int epfd;
int socketFd;

int createProcess(int num){
    printf("create process\n");
    if(num>0){
        int i=0;
        for(;i<num;i++){
            int pid=fork();
            if(pid>0){
                printf("parent pid=%d \n",getpid());
            }else if(pid==0){
                mIndex=i;
                printf("son pid=%d \n",getpid());
                break;
            }
        }
    }
    return 0;
}

int setNoBlock(int fd){
    int oldOption = fcntl(fd, F_GETFL);
    if(oldOption<0) {
        printf("fcntl error\n");
        exit(1);
    }
    int newOption = oldOption | O_NONBLOCK;
    fcntl(fd, F_SETFL, newOption);
    return oldOption;
}

void addFd(int epollfd, int fd){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}
//从epollfd标识的epoll内核事件表中删除fd上的所有注册事件
void removeFd(int epollfd, int fd){
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

void runParent(){
    //epfd=epoll_create(256);
    //addFd(epfd,socketFd);
    while(1){
        //printf("parent hello word\n");
        sleep(5);
    }
}

void runChild(){
    struct epoll_event events[MAX_EVENTS];
    char recvBuf[RECV_BUF_SIZE+1]= {0};
    int confd;
    int sockNum=0;
    epfd=epoll_create(256);
    addFd(epfd,socketFd);
    while(1){
        int num=epoll_wait(epfd,events,MAX_EVENTS,500);
        int i=0;
        for(;i<num;++i){
            int eventFd=events[i].data.fd;
            printf("the %d socket can be read %d \n",mIndex,eventFd);
            if((socketFd==eventFd)&&(events[i].events & EPOLLIN )){
                struct sockaddr_in clientAddr;
                socklen_t cntAddrLen = sizeof(clientAddr);
                confd = accept(socketFd, (struct sockaddr *)&clientAddr, &cntAddrLen);
                if (confd <= 0){
                    printf("the %d socket errno is: %d\n",mIndex, errno);
                    continue;
                }
                sockNum++;
                // printf("the %d socket num is: %d\n",mIndex, sockNum);
                addFd(epfd, confd);
                // removeFd(epfd, socketFd);
            }else if((socketFd!=eventFd)&&(events[i].events & EPOLLIN)){
                int len = recv(confd, recvBuf, RECV_BUF_SIZE,0);
                if(len>0){
                    printf("the %d message recv successful %s  len=%d\n", mIndex, recvBuf, len);
                }else if(len==0){
                    printf("the %d socket close\n", mIndex);
                }else{
                   // removeFd(epfd,eventFd);
                    printf("the %d recv error sockNum=%d errno=%d\n", mIndex, sockNum, errno);
                }
                memset(recvBuf,0,sizeof(recvBuf));
            }
        }
    }
}

int main(){
    printf("hello word\n");
    struct sockaddr_in serverAddr,clientAddr;

    socketFd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    inet_aton("127.0.0.1",&(serverAddr.sin_addr));
    serverAddr.sin_port=htons(8888);

    // 地址重用
    int opt= 1;
    if (-1 == setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int))){
        return -1;
    }
    setNoBlock(socketFd);
    bind(socketFd,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr));
    listen(socketFd,50);

    createProcess(2);

    if( mIndex != -1 ){
        runChild();
    }
    runParent();
}
