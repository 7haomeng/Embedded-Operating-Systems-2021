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

void write_action_to_server(int ret, int connfd, char *snd_action_to_server){
    
    if ((ret = write(connfd, snd_action_to_server, ret+1)) == -1){
        perror("Error : write action\n");
    }
    printf("client -> server : %s\t", snd_action_to_server);
}

void read_message_from_client(int ret, int connfd, char *rcv_from_client){
    memset(rcv_from_client, 0, BUFSIZE);
    if ((ret = read(connfd, rcv_from_client, BUFSIZE)) == -1){
        perror("Error : read\n");
    } 
}


int main(int argc, char *argv[]){

    int connfd, ret, i = 0;
    char rcv[BUFSIZE], client_action[BUFSIZE];

    if (argc != 6){
        printf("Usage: %s <host> <port> <deposit/withdraw> <amount> <times>\n", argv[0]);
        exit(-1);
    }

    connfd = connectsock(argv[1], argv[2], "tcp");

    ret = sprintf(client_action, "%s %s", argv[3], argv[4]);

    while (1)
    {
        if (i < atoi(argv[5])){
            printf("time = %d\t", i);
            write_action_to_server(ret, connfd, client_action);

            read_message_from_client(ret, connfd, rcv);
            printf("%s\n", rcv);
            if (strncmp(rcv, "ok\n", 2) == 0){
                i++;
            }
        }else{
            break;
        }
    }

    close(connfd);
    return 0;
}
