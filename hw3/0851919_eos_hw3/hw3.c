#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include "sockop.h"

#define BUFSIZE 1024

int pancake_machine = 0, blacktea_machine = 0, chief = 0;
int order_list = 0, total_order_num = 0, total_income = 0;
int total_pancake = 0, total_blacktea = 0;
int working_cnt = 0;
int receipt;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

void handler(int signum){
    int rc; 
    
    printf ("Main thread clean up mutex\n");
    rc = pthread_mutex_destroy(&mutex1);
    rc = pthread_mutex_destroy(&mutex2);
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void ctrlc_handler(int signum){
    printf("\n>> result.txt\n");

    FILE *fPtr = fopen("result.txt", "w");

    fprintf(fPtr,"customer: %d\n",total_order_num);
    fprintf(fPtr,"pancake: %d\n",total_pancake);
    fprintf(fPtr,"blacktea: %d\n",total_blacktea);
    fprintf(fPtr,"income: %d$\n",total_income);

    fclose(fPtr);
    exit(1);
}

void timer_handler(int signum){
    working_cnt++;
    printf("Timer expired %d times\n", working_cnt);
}

void read_message_from_client(int ret, int connfd, char *rcv_from_client){
    memset(rcv_from_client, 0, BUFSIZE);
    if ((ret = read(connfd, rcv_from_client, BUFSIZE)) == -1){
        perror("Error : read\n");
    }
}

int receipt_handler(int signum){
    
    int min = 10000000, max = 99999999;
    srand(time(NULL));
    receipt = rand() % (max - min + 1) + min;
    return receipt;
}

void write_menu_back_to_the_client(int ret, int connfd, char *snd_menu_to_client){
    ret = sprintf(snd_menu_to_client, "%s\n%s", "pancake:40$", "blacktea:30$");
    // printf("ret = %d\n", ret);
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_menu_to_client, ret+1)) == -1){
        perror("Error : write menu\n");
    }
    
    // printf("snd_menu_to_client = %s\n", snd_menu_to_client);
    memset(snd_menu_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void write_pbNum_back_to_the_client(int pancake_num, int bleaktea_num, int ret, int connfd, char *snd_num_to_client){
    ret = sprintf(snd_num_to_client, "p%db%d", pancake_num, bleaktea_num);
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_num_to_client, ret+1)) == -1){
        perror("Error : write pbNum\n");
    }
    
    // printf("snd_num_to_client = %s\n", snd_num_to_client);
    memset(snd_num_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void write_orderNum_back_to_the_client(int orderNum, int pancake_num, int bleaktea_num, int ret, int connfd, char *snd_orderNum_to_client){
    ret = sprintf(snd_orderNum_to_client, "%.04d|%d$", orderNum, ((pancake_num * 40) + (bleaktea_num * 30)));
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_orderNum_to_client, ret+1)) == -1){
        perror("Error : write orderNum\n");
    }
    
    // printf("snd_orderNum_to_client = %s\n", snd_orderNum_to_client);
    memset(snd_orderNum_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void write_done_pNum_back_to_the_client(int orderNum, int ret, int connfd, char *snd_done_pNum_to_client){
    ret = sprintf(snd_done_pNum_to_client, "%.04d|%s", orderNum, "pancake");
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_done_pNum_to_client, ret+1)) == -1){
        perror("Error : write done_pNum\n");
    }
    
    // printf("snd_done_pNum_to_client = %s\n", snd_done_pNum_to_client);
    memset(snd_done_pNum_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void write_done_bNum_back_to_the_client(int orderNum, int ret, int connfd, char *snd_done_bNum_to_client){
    ret = sprintf(snd_done_bNum_to_client, "%.04d|%s", orderNum, "blacktea");
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_done_bNum_to_client, ret+1)) == -1){
        perror("Error : write done_bNum\n");
    }
    
    // printf("snd_done_bNum_to_client = %s\n", snd_done_bNum_to_client);
    memset(snd_done_bNum_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void write_done_order_back_to_the_client(int orderNum, int ret, int connfd, char *snd_done_order_to_client){
    ret = sprintf(snd_done_order_to_client, "%.04d|%s", orderNum, "done");
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_done_order_to_client, ret+1)) == -1){
        perror("Error : write done_order\n");
    }
    
    // printf("snd_done_order_to_client = %s\n", snd_done_order_to_client);
    memset(snd_done_order_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void write_wait(int ret, int connfd, char *snd_wait_to_client){
    ret = sprintf(snd_wait_to_client, "wait or not?");
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_wait_to_client, ret+1)) == -1){
        perror("Error : write wait\n");
    }
    
    // printf("snd_wait_to_client = %s\n", snd_wait_to_client);
    memset(snd_wait_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void write_receipt_to_client(int ret, int connfd, int receipt, char *snd_receipt_to_client){
    ret = sprintf(snd_receipt_to_client, "your receipt is %d\n", receipt);
    pthread_mutex_lock(&mutex3);
    if ((ret = write(connfd, snd_receipt_to_client, ret+1)) == -1){
        perror("Error : write checkout\n");
    }
    
    printf("client = %s\n", snd_receipt_to_client);
    memset(snd_receipt_to_client, 0, BUFSIZE);
    pthread_mutex_unlock(&mutex3);
}

void *making_order_client_thread(int connfd_thread){

    int ret;
    char snd[BUFSIZE], rcv[BUFSIZE], str[BUFSIZE], rcp[BUFSIZE];

    int p_num_tmp = 0, b_num_tmp = 0;
    int p_num = 0, b_num = 0;
    int order_num = 0;
    int timer = -1, same_timer = 0;
    int pancake_doing = 0, blacktea_doing = 0;
    int pancake_machine_doing = 0, blacktea_machine_doing = 0, chief_doing = 0;
    // int receipt;
    bool receipt = false, id = false, work = true, doing_flag = false, timer_flag = false;

    while(work){

        /* read message from client */
        read_message_from_client(ret, connfd_thread, rcv);

        if(strncmp(rcv, "menu", 4) == 0){
            write_menu_back_to_the_client(ret, connfd_thread, snd);
        }

        if (strncmp(rcv, "pancake", 7) == 0){
            sscanf(rcv, "%*[^ ] %d", &p_num_tmp);
            p_num = p_num + p_num_tmp;
            write_pbNum_back_to_the_client(p_num, b_num, ret, connfd_thread, snd);
        }

        if (strncmp(rcv, "blacktea", 8) == 0){
            sscanf(rcv, "%*[^ ] %d", &b_num_tmp);
            b_num = b_num + b_num_tmp;
            write_pbNum_back_to_the_client(p_num, b_num, ret, connfd_thread, snd);
        }

        if (strncmp(rcv, "wait", 4) == 0){
            write_orderNum_back_to_the_client(order_num, p_num, b_num, ret, connfd_thread, snd);
            doing_flag = true;
        }

        if (strncmp(rcv, "cancel", 6) == 0){

            doing_flag = false;
            work = false;
            pthread_mutex_lock(&mutex1);
            order_list--;
            total_order_num--;
            total_income -= p_num*40+b_num*30;
            total_pancake += p_num;
            total_blacktea += b_num;
            pthread_mutex_unlock(&mutex1);
            break;
        }

        if (strncmp(rcv, "checkout", 8) == 0){
            pthread_mutex_lock(&mutex1);
            order_list++;
            total_order_num++;
            total_income += p_num*40+b_num*30;
            total_pancake += p_num;
            total_blacktea += b_num;
            pthread_mutex_unlock(&mutex1);
            order_num = total_order_num;

            if (order_list > 5){
                write_wait(ret, connfd_thread, snd);
                printf("wait or not?\n");
            }else{
                sleep(1);
                write_orderNum_back_to_the_client(order_num, p_num, b_num, ret, connfd_thread, snd);
                doing_flag = true;
                receipt = true;
            }
            
        }

        while(doing_flag)
        {   
            if (receipt){
                int sig;
                signal(SIGUSR1, receipt_handler);
                sleep(1);
                write_receipt_to_client(ret, connfd_thread, receipt_handler(sig), snd);
                kill(getpid(), SIGUSR1);
                receipt = false;
            }
            

            if(p_num == 0 && b_num == 0)
            {
                sleep(0.1);
                write_done_order_back_to_the_client(order_num, ret, connfd_thread, snd);
                work = false;
                doing_flag = false;
                timer_flag = false;
                receipt = false;
                break;
            }
            
            if(p_num > 0 && pancake_machine <= 2 && chief <= 3)
            {   
                if(pancake_doing == 0 && pancake_machine < 2 && chief < 3)
                {   
                    timer_flag = true;
                    pancake_machine_doing = 1;
                    pancake_doing = 1;
                    chief_doing += 1;
                }
                if(pancake_doing == 1 && working_cnt > 0 && working_cnt%3 == 0 && same_timer == 0)
                {
                    sleep(0.1);
                    write_done_pNum_back_to_the_client(order_num, ret, connfd_thread, snd);
                    p_num -= 1;
                    pancake_doing = 0;
                    pancake_machine_doing = -1;
                    chief_doing += -1;
                    same_timer = 1;
                }
            }
            
            if(b_num > 0 && blacktea_machine <= 2 && chief <= 3)
            {   
                if(blacktea_doing == 0 && blacktea_machine < 2 && chief < 3)
                {   
                    timer_flag = true;
                    blacktea_machine_doing = 1;
                    blacktea_doing = 1;
                    chief_doing += 1;
                }
                if(blacktea_doing == 1 && working_cnt > 0 && working_cnt%2 == 0 && same_timer == 0)
                {
                    sleep(0.1);
                    write_done_bNum_back_to_the_client(order_num, ret, connfd_thread, snd);
                    b_num -= 1;
                    blacktea_doing = 0;
                    blacktea_machine_doing = -1;
                    chief_doing += -1;
                    same_timer = 1; 
                }   
            }
            
            if(timer_flag)
            {
                pthread_mutex_lock(&mutex2);
                pancake_machine += pancake_machine_doing;
                blacktea_machine += blacktea_machine_doing;
                chief += chief_doing;
                pthread_mutex_unlock(&mutex2);
                pancake_machine_doing = 0;
                blacktea_machine_doing = 0;
                chief_doing = 0;
            }

            if(timer < working_cnt)
            {
                timer = working_cnt;
                same_timer = 0;
            }
        }

        if (work == false){
            pthread_mutex_lock(&mutex1);
            order_list--;
            pthread_mutex_unlock(&mutex1);
        }
    }  

    fgets(str, 10, stdin);
    if(strncmp(str, "exit", 4) == 0){
        printf("Total income $%d\n", total_income);
    }
    
    pthread_exit(NULL);
    close(connfd_thread);
}

int main(int argc, char *argv[]){

    int sockfd, connfd; /* socket descriptor */
    struct sockaddr_in addr_cln;
    socklen_t sLen = sizeof(addr_cln);

    pthread_mutex_init(&(mutex1),NULL);
    pthread_mutex_init(&(mutex2),NULL);
    pthread_mutex_init(&(mutex3),NULL);

    int i = 0, rc;
    pthread_t order_thread[32];

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = ctrlc_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    struct sigaction sa;
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    if (argc != 2){
        printf("Usage : %s port\n", argv[0]);
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

        // printf("thread[%d] create\n", i);
        rc = pthread_create(&order_thread[i], NULL, (void *)making_order_client_thread, connfd);
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
