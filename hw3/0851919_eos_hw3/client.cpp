#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

// #include <arpa/inet.h>
// #include <errno.h>
// #include <netdb.h>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <sys/wait.h>
#include <signal.h>

#include "sockop.h"

#define BUFSIZE 1024


int main(int argc, char *argv[]){

    int ret;
    int sockfd, connfd;
    char buf[BUFSIZE], menu[BUFSIZE];

    struct sockaddr_in sin;

    if(argc != 3){
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(-1);
    }

    while (1){
        /* create socket and connect to server */
        connfd = connectsock(argv[1], argv[2], "tcp") ;

        while(1){
            fgets(menu, 1024, stdin);

            if (strncmp(menu, "checkout", 8) == 0)
            {
                printf("Start Working\n");
                if((ret = write(connfd, menu, sizeof(menu))) == -1){
                    perror("Error : write\n");
                }
                memset(menu, 0,BUFSIZE);
                while(1){
                    if ((ret = read(connfd, buf, BUFSIZE)) == -1){
                        perror("Error : read\n");
                    }
                    
                    if (strncmp(buf, "done", 4) == 0){
                        break;
                    }
                    printf("%s\n", buf);

                }
            }
            
            else if((ret = write(connfd, menu, sizeof(menu))) == -1){
                perror("Error : write\n");
            }
            memset(menu, 0,BUFSIZE);
    
            memset(buf, 0,BUFSIZE);
            if ((ret = read(connfd, buf, BUFSIZE)) == -1){
                perror("Error : read\n");
            }
            printf("%s\n", buf);
        }       
    }
    close(connfd);
    
    return 0;
}
