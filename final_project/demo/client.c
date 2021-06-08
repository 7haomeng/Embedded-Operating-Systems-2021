#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include "sockop.h"

#define BUFSIZE 1024

void write_to_server(int ret, int connfd, char *snd_ID_to_server){
    
    if ((ret = write(connfd, snd_ID_to_server, 1024)) == -1){
        perror("Error : write action\n");
    }
    printf("[client] ID : %s\n", snd_ID_to_server);
    memset(snd_ID_to_server, 0, BUFSIZE);
}

void read_server_message(int ret, int connfd, char *read_server_msg){
    // memset(read_server_msg, 0, BUFSIZE);
    if ((ret = read(connfd, read_server_msg, BUFSIZE)) == -1){
        perror("Error : read\n");
    }
    printf("[client] ZC720 : %s\n\n", read_server_msg);
    memset(read_server_msg, 0, BUFSIZE);
}

int main(int argc, char *argv[]){

    int ret;
    int connfd;
    char rcv[BUFSIZE], snd[BUFSIZE];

    if(argc != 3){
        /* ./client <ip> <port> */
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(-1);
    }

    while(1){
        connfd = connectsock(argv[1], argv[2], "tcp") ;

        while(1){
            printf("Input your ID : ");
            fgets(snd, 1024, stdin);
            write_to_server(ret, connfd, snd);

            read_server_message(ret, connfd, rcv);
        }

    }

    close(connfd);
    return 0;
}
    