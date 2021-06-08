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

int total_income = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void handler(int signum) {
    int rc; 
    
    printf ("Main thread clean up mutex\n");
    rc = pthread_mutex_destroy(&mutex1);
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void write_ok_to_client(int ret, int connfd, char *snd_ok_to_client){
    ret = sprintf(snd_ok_to_client, "ok");
    if ((ret = write(connfd, snd_ok_to_client, ret+1)) == -1){
        perror("Error : write ok\n");
    }
    memset(snd_ok_to_client, 0, BUFSIZE);
}

void read_message_from_client(int ret, int connfd, char *rcv_from_client){
    memset(rcv_from_client, 0, BUFSIZE);
    if ((ret = read(connfd, rcv_from_client, BUFSIZE)) == -1){
        perror("Error : read\n");
    } 
}

void *ATM(int connfd_thread){

    int ret;
    char snd[BUFSIZE], rcv[BUFSIZE];

    int amount = 0;

    while(1){

        read_message_from_client(ret, connfd_thread, rcv);

        sscanf(rcv, "%*[^ ] %d", &amount);

        if(strncmp(rcv, "deposit", 7) == 0){
            pthread_mutex_lock(&mutex1);
            total_income += amount;
            printf("After deposit: %d\n", total_income);
            pthread_mutex_unlock(&mutex1);

            write_ok_to_client(ret, connfd_thread, snd);
        }

        if(strncmp(rcv, "withdraw", 8) == 0){
            pthread_mutex_lock(&mutex1);
            total_income -= amount;
            printf("After withdraw: %d\n", total_income);
            pthread_mutex_unlock(&mutex1);

            write_ok_to_client(ret, connfd_thread, snd);
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
    pthread_t order_thread[32];

    pthread_mutex_init(&(mutex1),NULL);

    if (argc != 2){
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
        
        if (connfd == -1){
            perror("Error : accept\n");
        }

        rc = pthread_create(&order_thread[i], NULL, (void *)ATM, connfd);
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