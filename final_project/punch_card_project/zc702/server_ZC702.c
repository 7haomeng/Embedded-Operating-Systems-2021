/*for socket*/
#include <arpa/inet.h> //htons, ntohs
#include <errno.h>     /* Errors */
#include <netdb.h>
#include <netinet/in.h> //struct sockaddr_in
#include <sys/socket.h> //the functions of socket
#include <sys/types.h>  /* Primitive System Data Types */

/*common*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> //sleep()
#include <time.h>

/*for vector*/
#include "server_vec.h"

/*for thread*/
#include <pthread.h>

/*for timer*/
#include <sys/time.h>

/*for signal*/
#include <signal.h>



#define BUFSIZE 1024
#define NUM_THREADS 5

// pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; //for person, m_pancake, m_black

//the information for each thread
typedef struct
{
    int t;      //thread number
    int connfd; //socket connfd
} info_transfer;


unsigned date = 0;                //今天日期
darray people;//記錄當天每個人的狀態
darray people_block;//紀錄被隔離者的狀態
darray location_clean;//紀錄今天被消毒的地區


void *BusyWork(void *info)
{

    info_transfer *my_info = (info_transfer *)info;

    int n;
    char snd[BUFSIZE], rcv[BUFSIZE];


    //start the punch
    while (1)
    {
        //read message from client
        if ((n = read(my_info->connfd, rcv, BUFSIZE)) == -1)
        {
            printf("Error: read()\n");
            break; //client is disconnected
        }

        //denote the userID
        int userID;
        userID = atoi(rcv);
        // printf("User ID: %d\n", userID);

        person_info pi;
        pi.userID = userID;
        pi.location = my_info->t;
        pi.status = 0;
        pi.day = 0;

        //確認此人的狀態
        bool flag_confirmed = false;
        person_info *ptr_people_block = darray_data(&people_block);
        int i;
        for (i=0; i<darray_size(&people_block); i++)
        {
            if (ptr_people_block[i].userID == pi.userID)
            {
                flag_confirmed = true;
                switch (ptr_people_block[i].status)
                {
                case 1: //confirmed
                    pi.status = ptr_people_block[i].status;
                    pi.day = ptr_people_block[i].day;
                    break;
                case 2: //isolation
                    pi.status = ptr_people_block[i].status;
                    pi.day = ptr_people_block[i].day;
                    break;
                default:
                    break;
                }

                break;
            }
        }

        //確認此人今天是否重複打卡
        bool flag = false;
        person_info *ptr_people = darray_data(&people);
        for (i=0; i<darray_size(&people); i++)
        {
            if (ptr_people[i].userID == pi.userID)
            {
                flag = true;
                break; //the person has been other place
            }
        }

        //計算此區域目前人數
        bool flag_exceed = false;
        int count = 0;
        for (i=0; i<darray_size(&people); i++)
        {
            if (ptr_people[i].location == my_info->t && count < 5)
            {
                count++;
            }
        }

        if (count >= 5)
        {
            flag_exceed = true;
        }

        //確認此區域是否要消毒
        person_info *ptr_location_clean = darray_data(&location_clean);
        bool flag_clean = false;
        for (i=0; i<darray_size(&location_clean); i++)
        {
            if (ptr_location_clean[i].location == my_info->t)
            {
                flag_clean = true;
                break;
            }
        }

        //決定要回傳甚麼給client
        memset(snd, 0, BUFSIZE); //clean the string
        n = sprintf(snd, "%d,%u,%d,%d,%d,%d,%d,%d", pi.userID, date, pi.location, count, flag_clean, flag, pi.status, pi.day);

        if (!flag && !flag_confirmed && !flag_exceed && !flag_clean)
        { //此人和此地無任何事故
            darray_append(&people, pi);
        }

        //write message to client
        if ((n = write(my_info->connfd, snd, strlen(snd) + 1)) == -1)
        {
            printf("Thread [%d] Error: write()\n", my_info->t);
        }
    }

    close(my_info->connfd);

    pthread_exit((void *)0);
}

void timer_handler(int signum)
{

    //update date
    date++;
    printf("day: %u\n", date);

    //更新隔離者們的狀態
    person_info *ptrs_people_block = darray_data(&people_block);
    int it=0;
    while (it < darray_size(&people_block))
    {
        switch (ptrs_people_block[it].status)
        {
        case 0: //normal
            darray_erase(&people_block, it);
            break;
        case 1: //confirmed
            if (ptrs_people_block[it].day == 1)
            {
                darray_erase(&people_block, it);
            }
            else
            {
                ptrs_people_block[it].day--;
                it++;
            }
            break;
        case 2: //isolation
            if (ptrs_people_block[it].day == 1)
            {
                darray_erase(&people_block, it);
            }
            else
            {
                ptrs_people_block[it].day--;
                it++;
            }
            break;
        default:
            break;
        }
        
    }


    //確認是否有確診者，並匡列接觸者
    int is_confirmed;
    is_confirmed=rand()%2; //0: not confirmed; 1:confirmed

    bool flag_confirmed = false;
    int location_confirmed = -1;

    
    // printf("check people: ");
    // for (i=0; i<darray_size(&people); i++)
    // {
    //     printf("%d, ", ptrs_people[i].userID);
    // }
    
    
    if(is_confirmed && darray_size(&people)>0){
        int i;
        person_info *ptrs_people = darray_data(&people);

        int index_confirmed;
        index_confirmed=rand()%darray_size(&people);

        printf("People ID Confirmed: %d\n", ptrs_people[index_confirmed].userID);

        for (i=0; i<darray_size(&people); i++)
        {
            // printf("%d, ", ptrs_people[i].userID);
            if (ptrs_people[i].userID == ptrs_people[index_confirmed].userID)
            {
                flag_confirmed = true;
                location_confirmed = ptrs_people[i].location;
                break;
            }
        }

        if (flag_confirmed)
        {
            for (i=0; i<darray_size(&people); i++)
            {
                if (ptrs_people[i].userID == ptrs_people[index_confirmed].userID)
                {
                    ptrs_people[i].status = 1; //confirmed
                    ptrs_people[i].day = 5;
                    darray_append(&people_block, ptrs_people[i]);
                }
                else if (ptrs_people[i].location == location_confirmed)
                {
                    ptrs_people[i].status = 2;
                    ptrs_people[i].day = 3;
                    darray_append(&people_block, ptrs_people[i]);
                }
            }
        }
    }

    darray_erase_all(&people);


    //更新藥消毒的區域
    darray_erase_all(&location_clean);
    person_info strcut_location_clean;
    strcut_location_clean.location=location_confirmed;
    if (location_confirmed != -1)
    {
        darray_append(&location_clean, strcut_location_clean);
        printf("Blocked Location: %d\n", location_confirmed);
    }

    printf(".....................\n");
}

int passivesock(const char *service, const char *transport, int qlen)
{
    struct servent *pse;    /* pointer to service information entry */
    struct sockaddr_in sin; /* an Internet endpoint address */
    int s, type;            /* socket descriptor and socket type */

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
    if ((pse = getservbyname(service, transport)))
        sin.sin_port = htons(ntohs((unsigned short)pse->s_port));
    else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
        printf("Can’t find \"%s\" service entry\n", service);

    /* Use protocol to choose a socket type */
    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM; //
    else
        type = SOCK_STREAM; //

    /* Allocate a socket */
    s = socket(PF_INET, type, 0);
    if (s < 0)
        printf("Can't create socket : %s\n", strerror(errno));

    /* Bind the socket */
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        printf("Can't bind to port %s : %s\n", service, strerror(errno));

    /* Set the maximum number of waiting connection */
    if (type == SOCK_STREAM && listen(s, qlen) < 0)
        printf("Can't listen on port %s : %s\n", service, strerror(errno));

    return s;
}

int main(int argc, char *argv[])
{
    darray_create(&people);
    darray_create(&people_block);
    darray_create(&location_clean);

    /*set time for random number*/
    srand(time(0));

    /*set timer to supervise the workers*/
    struct sigaction sa;
    struct itimerval timer;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = 20;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 20;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);



    /*set socket*/
    int sockfd, connfd; /* socket descriptor */
    struct sockaddr_in addr_cln;
    socklen_t sLen = sizeof(addr_cln);

    if (argc != 2)
        printf("Usage: %s port\n", argv[0]);

    //create socket and bind socket to port
    sockfd = passivesock(argv[1], "tcp", 10);

    /*set thread*/
    pthread_t thread[NUM_THREADS + 1];
    pthread_attr_t attr;
    int rc;
    int t = 0;
    //initialize and set thread detached atrribute
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    info_transfer info[NUM_THREADS];
    while (1)
    {
        /* waiting for connection */
        //if a new client is created and linked to the server, accept
        connfd = accept(sockfd, (struct sockaddr *)&addr_cln, &sLen);
        if (connfd == -1)
        {
            // printf("Error : accept ()\n");
            continue; //the checker is disconnected
        }

        /* now create new thread */
        //the infomation will be transferred to the thread
        info[t].connfd = connfd; //the new thread's connfd
        info[t].t = t;           //tread number

        // printf("Main: creating thread %ld\n", info[t].t);
        rc = pthread_create(&thread[t], &attr, BusyWork, (void *)&info[t]);
        if (rc)
        {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

        t++;
    }

    darray_destroy(&people);
    darray_destroy(&people_block);
    darray_destroy(&location_clean);

    return 0;
}
