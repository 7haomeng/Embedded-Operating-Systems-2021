#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define SHMSZ   27
int upperbd;
int lowerbd = 1;
pid_t pidd;

typedef struct {
    int guess;
    char result[8];
}data;

data *datamap;
int guessnum;

void timer_handler( int signum )
{
    if((upperbd+lowerbd)%2 != 0){
        guessnum = (upperbd+lowerbd)/2;
    }
    else{
        guessnum = (upperbd+lowerbd)/2;
    }
    (* datamap).guess  = guessnum;
    printf("[game] Guess: %d\n",guessnum);
    kill(pidd,SIGUSR1);
}

void handler( int signo , siginfo_t* info, void *context)
{
    if(strcmp(datamap->result,"smaller") == 0){
        upperbd = guessnum;
        //printf("ss\n");
    }
    else if(strcmp(datamap->result,"bigger") == 0){
        lowerbd = guessnum;
        //printf("bb\n");
    }
    else if(strcmp(datamap->result,"bingo") == 0){
        // printf("Client detach the share memory.\n") ;
        shmdt(datamap) ;
        exit(0);
    }
}
int main(int argc , char *argv[])
{ 
    int shmid;
    key_t key;
    int key_temp;
    char *shm , *s;
    struct sigaction sa ;
    struct sigaction my_action ;
    struct itimerval timer ;

    key_temp = atoi(argv[1]);
    key = (key_t)key_temp;
    upperbd = atoi(argv[2]);
    pidd = (pid_t)atoi(argv[3]);

    /* Locate the segment */
    if((shmid = shmget(key , SHMSZ, 0666)) < 0){
        perror( "shmget  ");
        exit(1);
    }
    /* Now we attach the segment to our data space */
    datamap = (data *)shmat(shmid, NULL, 0);
    // printf("Client attach the share memory created by server.\n") ;

    /* Install timer_handler as the signal handler for SIGVTALRM */
    memset (&sa, 0, sizeof(sa)) ;
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    memset(&my_action , 0 , sizeof( struct sigaction)) ;
    my_action.sa_flags = SA_SIGINFO ;
    my_action.sa_sigaction = handler;
    /* Configure the timer to expire after 250 msec */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec =500000;
    /* Reset the timer back to 250 msec after expired */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 500000;
    setitimer(ITIMER_VIRTUAL, &timer, NULL) ;
    sigaction( SIGUSR1 , &my_action , NULL ) ;
    while(1);
    /* Now read what the server put in the memory */
    // printf("Client read characters from share memory ...\n") ;

    // printf("Client detach the share memory.\n") ;
    shmdt(datamap) ;
    return 0 ;
}