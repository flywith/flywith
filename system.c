#include<stdio.h>
#include<stdlib.h>

void main(){
    int stat=system("./hello");
    printf("pid=%d %d\n",getpid(),stat);
}
