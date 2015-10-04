#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main()
{
    int socketFd;
    char buf[20]="hello world";
    char buffer[20];
    memset(buffer,0,sizeof(buffer));
    strcpy(buffer,buf);
    //printf("size=%d \n",sizeof(buffer));
    struct sockaddr_in cnt_addr;
    printf("Hello,welcome to client !\n");

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == socketFd){
        printf("socket fail !\n");
        return -1;
    }

    bzero(&cnt_addr,sizeof(struct sockaddr_in));
    cnt_addr.sin_family=AF_INET;
    cnt_addr.sin_addr.s_addr= inet_addr("127.0.0.1");
    cnt_addr.sin_port=htons(8888);
    printf("cnt_addrr = %#x ,port : %#x\n",cnt_addr.sin_addr.s_addr,cnt_addr.sin_port);

//    int val = fcntl(socketFd, F_GETFL, 0);
//    fcntl(socketFd, F_SETFL, val | O_NONBLOCK); //设置套接口非阻塞

    int conFlag = connect(socketFd,(struct sockaddr *)(&cnt_addr), sizeof(struct sockaddr));
//    sleep(5);
    if(conFlag<0){
        printf("connect fail !\n");
        return -1;
    }
    while(1){
        if(-1==send(socketFd, buffer, strlen(buffer)+1, 0)){
            printf("read data fail !\r\n");
            return -1;
        }
        sleep(2);
    }
    close(socketFd);
    return 0;
}
