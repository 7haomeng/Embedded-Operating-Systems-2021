#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include "sockop.h"

#define BUFSIZE 1024

void handler(int signum){

    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void write_ok_to_client(int ret, int connfd, char *snd_ok_to_client){
    ret = sprintf(snd_ok_to_client, "打卡機封鎖與消毒 / 打卡機位置與時間");
    if ((ret = write(connfd, snd_ok_to_client, 1024)) == -1){
        perror("Error : write ok\n");
    }
    memset(snd_ok_to_client, 0, BUFSIZE);
}

void read_message_from_client(int ret, int connfd, char *rcv_from_client){
    // memset(rcv_from_client, 0, BUFSIZE);
    if ((ret = read(connfd, rcv_from_client, BUFSIZE)) == -1){
        perror("Error : read\n");
    } 
    // printf("read_message_from_client : %s\n", rcv_from_client);
}

void *punch_card_machine(int connfd_thread){

    int ret;
    char snd[BUFSIZE], rcv[BUFSIZE];

    // int ID_code;
    char ID_code[BUFSIZE];

    while(1){

        read_message_from_client(ret, connfd_thread, rcv);

        printf("ID_code : %s\n", rcv);

        sscanf(rcv, "%s", ID_code);
        // printf("rcv : %s\n", rcv);

        if (rcv != NULL){
            printf("[server] ID : %s\n\n", ID_code);
            sleep(1);
            // write_ok_to_client(ret, connfd_thread, snd);
        }

    }

    pthread_exit(NULL);
    close(connfd_thread);
}

int main(int argc, char *argv[]){

    int sockfd, connfd; /* socket descriptor */
    struct sockaddr_in addr_cln;
    socklen_t sLen = sizeof(addr_cln);

    int i = 0, rc;
    pthread_t punch_card_machine_thread[4];

    if (argc != 2){
        /* ./server <port>*/
        printf("Usage : %s <port>\n", argv[0]);
        exit(-1);
    }

    /* create socket and bind socket to port */
    sockfd = passivesock(argv[1], "tcp", 100);
    if (sockfd < 0){
        perror("Can't create socket\n");
    }

    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    while(1){
        /* waiting for connection */
        connfd = accept(sockfd, (struct sockaddr * ) &addr_cln, &sLen);
        
        // if(connfd > 0){
        //     printf("ok\n");
        // }
        
        
        if (connfd == -1){
            perror("Error : accept\n");
        }

        rc = pthread_create(&punch_card_machine_thread[i], NULL, (void *)punch_card_machine, connfd);
        if (rc){
            printf("ERROR : return code from pthread create() is %d\n", rc) ;
            exit(-1);
        }    
        i++;

        signal(SIGCHLD, handler);
    }
    pthread_exit(NULL);
    close (connfd);
    close(sockfd);

    return 0;

}