#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

int main(int argc, char *argv[])
{
    unsigned short key;
    int fd, ret;

    if ((fd = open("/dev/lcd", O_RDWR)) < 0)
    {
        printf("Open /dev/lcd faild.\n");
        return (-1);
    }

    ioctl(fd, KEY_IOCTL_CLEAR, key);

    while(1)
    {
        ret = ioctl(fd, KEY_IOCTL_CHECK_EMTPY, &key);
        if (ret < 0)
        {
            sleep(1);
            continue;
        }
        ret = ioctl(fd, KEY_IOCTL_GET_CHAR, &key);
        // ioctl(fd, KEY_IOCTL_GET_CHAR, &key);

        if ((key & 0xff) == '1')
        {
            printf("Keypad '1' \n");
            continue;
        }

        if ((key & 0xff) == '2')
        {
            printf("Keypad '2' \n");
            continue;
        }

        if ((key & 0xff) == '3')
        {
            printf("Keypad '3' \n");
            continue;
        }

        if ((key & 0xff) == '4')
        {
            printf("Keypad '4' \n");
            continue;
        }

        if ((key & 0xff) == '5')
        {
            printf("Keypad '5' \n");
            continue;
        }

        if ((key & 0xff) == '6')
        {
            printf("Keypad '6' \n");
            continue;
        }

        if ((key & 0xff) == '7')
        {
            printf("Keypad '7' \n");
            continue;
        }

        if ((key & 0xff) == '8')
        {
            printf("Keypad '8' \n");
            continue;
        }

        if ((key & 0xff) == '9')
        {
            printf("Keypad '9' \n");
            continue;
        }

        if ((key & 0xff) == '0')
        {
            printf("Keypad '0' \n");
            continue;
        }

        if ((key & 0xff) == 'A')
        {
            printf("Keypad '+' \n");
            continue;
        }
        
        if ((key & 0xff) == 'B')
        {
            printf("Keypad '-' \n");
            continue;
        }

        if ((key & 0xff) == 'C')
        {
            printf("Keypad '*' \n");
            continue;
        }

        if ((key & 0xff) == 'D')
        {
            printf("Keypad '/' \n");
            continue;
        }

        if ((key & 0xff) == '*')
        {
            printf("Keypad Clear \n");
            continue;
        }

        if ((key & 0xff) == '#')
        {
            printf("Keypad '=' \n");
            break; /* terminate*/
        }

        close(fd);
    }

    return 0;
}