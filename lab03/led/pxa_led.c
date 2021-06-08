#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

int main(int argc, char *argv[])
{
    int fd;
    int retval;

    unsigned short data;

    /* Open device /dev/lcd */
    if ((fd = open("/dev/lcd", O_RDWR)) < 0)
    {
        printf("Open /dev/lcd faild.\n");
        exit(-1);
    }

    /* Turn on all LED lamps */
    data = LED_ALL_ON;
    ioctl(fd, LED_IOCTL_SET, &data);
    printf("Turn on all LED lamps.\n");
    sleep(3);

    /* Turn off all LED lamps */
    data = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &data);
    printf("Turn off all LED lamps.\n");
    sleep(3);

    /* Turn on D9 */
    data = LED_D9_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    printf("Turn on D9.\n");
    // sleep(3);

    /* Turn on D10 */
    data = LED_D10_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    printf("Turn on D10.\n");
    sleep(3);

    /* Turn off D9 */
    data = LED_D9_INDEX;
    ioctl(fd, LED_IOCTL_BIT_CLEAR, &data);
    printf("Turn off D9.\n");
    sleep(3);

    /* Turn off D10 */
    data = LED_D10_INDEX;
    ioctl(fd, LED_IOCTL_BIT_CLEAR, &data);
    printf("Turn off D10.\n");
    sleep(3);

    /* Close fd */
    close(fd);

    return 0;
}