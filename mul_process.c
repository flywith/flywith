#include<stdio.h>
#include<stdlib.h>
int CreateProcess(int num){
                printf("create process\n");
    if(num>0){
        int i=0;
        for(;i<num;i++){
            int pid=fork();
            if(pid>0){
                printf("parent pid=%d \n",getpid());
            }else if(pid==0){
                printf("son pid=%d \n",getpid());
                break;
            }
        }
    }
    return 0;
}

void main(){
    printf("hello word\n");
    CreateProcess(5);
    while(1){
        printf("pid=%d \n",getpid());
        sleep(2);
    }

}
