#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define SHMSZ 27

typedef struct {
    int guess;
    char result[8];
}data;
int ans;
data *datamap;
int flag;

void handler( int signo , siginfo_t* info, void *context)
{
    int guessnum;
    pid_t pid;
    int retval;
     /* Now put some things into the memory for the other process to read */
    guessnum = (* datamap).guess;
    if(guessnum > ans){
        strcpy((datamap)->result,"smaller");
        printf("[game] Guess: %d, %s",guessnum,"smaller\n");
    }
    else if(guessnum < ans){
        strcpy((datamap)->result,"bigger");
        printf("[game] Guess: %d, %s",guessnum,"bigger\n");
    }
    else{
        strcpy((datamap)->result,"bingo");
        printf("[game] Guess: %d, %s",guessnum,"bingo\n");
        pid = info->si_pid;
        kill(pid,SIGUSR1);
        sleep(1);
        shmdt(datamap) ;
         /* Destroy the share memory segment */
        // printf("Server destroy the share memory.\n") ;
        retval = shmctl (flag , IPC_RMID , NULL) ;
        if(retval < 0)
        {
            fprintf(stderr , "Server remove share memory failed\n" ) ;
            exit(1);
        }
        exit(0);
        
    }
    pid = info->si_pid;
    kill(pid,SIGUSR1);
}

int main(int argc , char *argv[])
{
    char c ;
    int shmid;
    char *shm, *s;
    int retval;
    key_t key;
    int key_temp;
    char temp[8];
    int guessnum;
    struct sigaction my_action ;
    /* We'll name our shared memory segment "5678" */
    key_temp = atoi(argv[1]);
    key = (key_t)key_temp;
    ans =atoi(argv[2]);
    //printf("%d\n",key);
    /* Create the segment */
    if((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0){
        perror( "shmget") ;
        exit(1);
    }
    flag = shmid;
    /* Now we attach the segment to our data space */
    datamap = (data *)shmat(shmid, NULL, 0);
    // printf( "Server create and attach the share memory.\n") ;
    /* register handler to SIGUSR1 */
    memset(&my_action , 0 , sizeof( struct sigaction)) ;
    my_action.sa_flags = SA_SIGINFO ;
    my_action.sa_sigaction = handler;
    printf("[game] Game PID: %d\n", getpid());
    // printf( "Process (%d) is catching SIGUSR1...\n" , getpid());
    sigaction(SIGUSR1 , &my_action , NULL ) ;
    while(1);

    return 0 ;
}