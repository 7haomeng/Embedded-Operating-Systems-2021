#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>

#define BUFSIZE 1024;

void handler(int signum) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]){

    int sockfd, connfd; /* socket descriptor */
    struct sockaddr_in addr_cln, sin;
    socklen_t sLen = sizeof(addr_cln);

    pid_t child_pid;/* variable to store the child ’s pid */

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    if (argc != 2){
        printf("Usage : %s port\n", argv[0]);
        exit(-1);
    }

    /* Map service name to port number */
    sin.sin_port = htons((unsigned short)atoi(argv[1]));

    /* Allocate a socket */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("Can't create socket\n");
    }

    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    /* Bind the socket */
    if (bind(sockfd, (struct sockaddr * ) &sin, sizeof(sin)) < 0){
        perror("Can't bind to port\n");
    }

    /* Set the maximum number of waiting connection */
    listen(sockfd, 10);

    signal(SIGCHLD, handler);

    while(1){
        /* waiting for connection */
        connfd = accept(sockfd, (struct sockaddr * ) &addr_cln, &sLen);
        
        if(connfd == -1){
            perror("Error : accept\n");
        }

        /* now create new process */
        child_pid = fork();

        /* fork succeeded */
        if (child_pid >= 0){
            if (child_pid == 0){
                dup2(connfd, STDOUT_FILENO);
                
                execlp("sl", "sl", "-l", NULL);

            }else{
                printf("Train ID : %d\n", (int)child_pid);
            }
        }
        close (connfd);
    }

    close(sockfd);
    return 0;
}
