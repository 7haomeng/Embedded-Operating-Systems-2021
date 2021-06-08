#include <cstdio>       // fprintf(), perror()
#include <cstdlib>      // exit()
#include <cstring>      // memset()
#include <csignal>      // signal()
#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()

#include <sys/socket.h> // socket(), connect()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // htons()

int fd;

void sigint_handler(int sig) 
{
    close(fd);
}

int main(int argc, char *argv[]) 
{

    char name_HAO;

    // if(argc != 2) 
    // {
    //     fprintf(stderr, "Usage: ./writer <name>");
    //     exit(EXIT_FAILURE);
    // }

    // Signal callback
    signal(SIGINT, sigint_handler);

    // Open driver
    if((fd = open("/dev/mydev", O_RDWR)) < 0) 
    {
        printf("Open /dev/mydev faild.\n");
        exit(-1);
    }

    int ret;
    int i = 0;

    for(i=0; i<3; i++) 
    {
        // if(argv[1][i] == 'b' || argv[1][i] == 'd' || argv[1][i] == 'q')
        //     name_HAO = (argv[1][i] - 'a');
        // else
        //     name_HAO = (argv[1][i] - 'A');
        name_HAO = (argv[1][i] - 'A');

        printf("%d\n", name_HAO);

        // ret = write(fd, &name_HAO, sizeof(name_HAO));

        if((ret = write(fd, &name_HAO, sizeof(name_HAO))) == -1)
        {
            perror("write()");
            exit(EXIT_FAILURE);
        }

        sleep(1);
    }

    close(fd);
    return 0;
}