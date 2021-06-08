#include <arpa/inet.h> //htons, ntohs
#include <errno.h>     /* Errors */
#include <netdb.h>
#include <netinet/in.h> //struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h> //the functions of socket
#include <sys/types.h>  /* Primitive System Data Types */

#include <unistd.h> //sleep()

/*for signal*/
#include <signal.h>


#define BUFSIZE 1024

int connectsock(const char *host, const char *service, const char *transport)
{
    struct hostent *phe;
    struct servent *pse;
    struct sockaddr_in sin;
    int s, type;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    /* Map service name to port number */
    if ((pse = getservbyname(service, transport)))
        sin.sin_port = pse->s_port;
    else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
        printf("Can’t get \"%s\" service entry\n", service);

    if ((phe = gethostbyname(host)))
    {
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    }
    else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
    {
        printf("Can't get \"%s\" host entry\n", host);
    }

    if (strcmp(transport, "udp") == 0)
    {
        type = SOCK_DGRAM;
    }
    else
    {
        type = SOCK_STREAM;
    }

    /* Allocate a socket */
    s = socket(PF_INET, type, 0);
    if (s < 0)
        printf("Can't create socket : %s\n", strerror(errno));

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        printf("Can't connect to %s.%s: %s\n", host, service, strerror(errno));
    }

    return s;
}






int main(int argc, char *argv[])
{

    /*socket*/
    int connfd;
    int n;
    char snd[BUFSIZE], buf[BUFSIZE];

    if (argc != 3)
    {
        printf("Ussage: %s host_address host_port\n", argv[0]);
    }

    connfd = connectsock(argv[1], argv[2], "tcp");


    //start the punch
    int userID;
    printf("Please input your userID: ");

    while (scanf("%d", &userID))
    {
        memset(snd, 0, BUFSIZE); //clean the string
        n = sprintf(snd, "%d", userID);
        //write message to client
        if ((n = write(connfd, snd, strlen(snd) + 1)) == -1)
        {
            printf("Error: write()\n");
        }

        //read message from server
        memset(buf, 0, BUFSIZE);
        if ((n = read(connfd, buf, BUFSIZE)) == -1)
        {
            printf("Error: read()\n");
        }

        //將收到的string解成int info[8]
        char *sptr;
        sptr = buf;
        int info[8];
        int i;
        for (i = 0; i < 8; i++)
        {
            int num = 0;
            while (*sptr != ',' && *sptr != '\0')
            {
                num = num * 10 + (int)(*sptr - '0');
                sptr++;
            }

            sptr++;

            info[i] = num;
        }

        //根據info[8]印出相應的內容在terminal上
        if (info[4] == 1)
        { //under clean
            printf("This location is blocked.\n");

            if (info[6] == 0 && info[5] == 1)
            {
                printf("You have punched today\n");
            }
            else if (info[6] != 0)
            { //check in fail
                printf("You are isolated for %d day(s)\n", info[7]);
            }

        }
        else if (info[6] == 0 && info[3] != 5 && info[5] == 0)
        { //check in success
            printf("ID %d check in success\n", info[0]);
        }
        else if (info[6] == 0 && info[5] == 1)
        {
            printf("ID %d check in fail. You have punched today\n", info[0]);
        }
        else if (info[6] == 0 && info[3] == 5)
        {
            printf("ID %d check in fail. The location has had 5 people\n", info[0]);
        }
        else if (info[6] != 0)
        { //check in fail
            printf("ID %d check in fail. You are isolated for %d day(s)\n", info[0], info[7]);
        }

        printf("............................\nPlease input your userID: ");
    }

    close(connfd);

    return 0;
}

