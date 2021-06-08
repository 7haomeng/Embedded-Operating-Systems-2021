#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

int main(int argc, char *argv[])
{
    int fd;

    /* struct for saving LCD data */
    lcd_write_info_t display;

    if ((fd = open("/dev/lcd", O_RDWR)) < 0)
    {
        printf("Open /dev/lcd faild.\n");
        return (-1);
    }

    /* Clear LCD */
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);

    /* Save output string to display data structure */
    display.Count = sprintf((char *) display.Msg, "Hello World\n");
    /* Print out "Hello World" to LCD */
    ioctl(fd, LCD_IOCTL_WRITE, &display);

    /* Get the cursor position */
    ioctl(fd, LCD_IOCTL_CUR_GET, &display);
    printf("The cursor position is at (x, y) = (%d, %d)\n", display.CursorX, display.CursorY);
    sleep(3);

    // ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    close(fd);

    return 0;
}