#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

#define MAIN "1. menu\n2. order"
#define MENU "pancake    $40\nblack tea  $30"
#define ORDER "1. show\n2. pancake\n3. black tea\n4. check out\n5. cancel\n"
#define NUM "num : "

void clear(int fd, lcd_write_info_t lcd, _7seg_info_t _7seg, unsigned short key, unsigned short led)
{
    led = LED_ALL_OFF;
    _7seg.Mode = _7SEG_MODE_HEX_VALUE;
    _7seg.Which = _7SEG_ALL;
    _7seg.Value = 0;
    
    ioctl(fd, LED_IOCTL_SET, &led);
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    ioctl(fd, KEY_IOCTL_CLEAR, key);
    ioctl(fd, _7SEG_IOCTL_ON, NULL);
    ioctl(fd, _7SEG_IOCTL_SET, &_7seg);
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);

    printf("Start\n");
}

void display_lcd(int fd, lcd_write_info_t lcd){
    ioctl(fd, LCD_IOCTL_WRITE, &lcd);
}

void display_7seg(int fd, _7seg_info_t _7seg, int no)
{
    _7seg.Mode = _7SEG_MODE_HEX_VALUE;
    _7seg.Which = _7SEG_ALL;
    _7seg.Value = (no/100)*256 + ((no%100)/10)*16 + no%10;
    ioctl(fd, _7SEG_IOCTL_SET, &_7seg);
}

void Main(int fd, int mode, lcd_write_info_t lcd){
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    lcd.Count = sprintf((char *) lcd.Msg, MAIN);
    display_lcd(fd, lcd);
    printf("Show Main\n");
}

void Menu(int fd, int mode, lcd_write_info_t lcd){
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    lcd.Count = sprintf((char *) lcd.Msg, MENU);
    display_lcd(fd, lcd);
    printf("Show Menu\n");
}

void Order(int fd, int mode, lcd_write_info_t lcd){
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    lcd.Count = sprintf((char *) lcd.Msg, ORDER);
    display_lcd(fd, lcd);
    printf("Show Order\n");
}

void Order_Show(int fd, lcd_write_info_t lcd, int pancake_total, int blacktea_total){
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    lcd.Count = sprintf((char *)lcd.Msg, "pancake   %d\nblack tea %d\n\n$%d|%ds", pancake_total, blacktea_total, pancake_total*40+blacktea_total*30, pancake_total*5>=blacktea_total*3?pancake_total*5:blacktea_total*3);
    display_lcd(fd, lcd);
    printf("pancake   %d\nblack tea %d\n\n$%d|%ds", pancake_total, blacktea_total, pancake_total*40+blacktea_total*30, pancake_total*5>=blacktea_total*3?pancake_total*5:blacktea_total*3);
}

void Order_Num(int fd, lcd_write_info_t lcd, int num){
    if(num != 0)
    {
        lcd.Count = sprintf((char *)lcd.Msg, "%d", num);
        display_lcd(fd, lcd);
    }
}

void Check_Out(int fd, lcd_write_info_t lcd, int no_num, int pancake_total, int blacktea_total, int pancake_done, int blacktea_done){
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    lcd.Count = sprintf((char *)lcd.Msg, "No.%04d\npancake   %d/%d\nblack tea %d/%d\nfee: $%d", no_num, pancake_done, pancake_total, blacktea_done, blacktea_total, pancake_total*40+blacktea_total*30);
    display_lcd(fd, lcd);
    printf("No.%04d\npancake   %d/%d\nblack tea %d/%d\nfee: $%d", no_num, pancake_done, pancake_total, blacktea_done, blacktea_total, pancake_total*40+blacktea_total*30);
}

int main(){
    int fd, ret, mode;
    int timer = -1;
    int no_num = 1;
    int pancake_num = 0, blacktea_num = 0;
    int pancake_tmp = 0, blacktea_tmp = 0;
    // int pancake_done = 0, blacktea_done = 0;
    int pancake_done_num = -1, blacktea_done_num = -1;
    bool led_cnt = false;
    unsigned short key;
    unsigned short led;
    unsigned short led_data;
    _7seg_info_t _7seg;
    lcd_write_info_t lcd;

    /* Open device /dev/lcd */
    if((fd = open("/dev/lcd",O_RDWR)) == -1) {
    	printf("Open /dev/lcd faild.\n");
    	exit(-1);
    }

    /*Clear device*/
    clear(fd, lcd, _7seg, key, led);

    Main(fd, mode, lcd);
    mode = 0;
    while(1){
        ret = ioctl(fd, KEY_IOCTL_CHECK_EMTPY, &key);
        if(ret<0){
        	sleep(1);
        	continue;
        }
        ret = ioctl(fd, KEY_IOCTL_GET_CHAR,&key);

        switch (key & 0xff){
            case '1' :
            switch (mode){
                case 0 :
                Menu(fd, mode, lcd);
                mode = 1;
                break;

                case 1 :
                Main(fd, mode, lcd);
                mode = 0;
                break;

                case 2 :
                Order_Show(fd, lcd, pancake_num, blacktea_num);
                mode = 3;
                break;

                case 3 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 4 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                pancake_tmp *= 10;
                pancake_tmp += 1;
                Order_Num(fd, lcd, pancake_tmp);
                break;

                case 5 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                blacktea_tmp *= 10;
                blacktea_tmp += 1;
                Order_Num(fd, lcd, blacktea_tmp);
                break;

                case 6 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

            }

            break;
                
            case '2' :
            switch(mode){
                case 0 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 1 :
                Main(fd, mode, lcd);
                mode = 0;
                break;

                case 2 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                mode = 4;
                break;

                case 3 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 4 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                pancake_tmp *= 10;
                pancake_tmp += 2;
                Order_Num(fd, lcd, pancake_tmp);
                break;

                case 5 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                blacktea_tmp *= 10;
                blacktea_tmp += 2;
                Order_Num(fd, lcd, blacktea_tmp);
                break;

                case 6 :
                Order(fd, mode, lcd);
                mode = 2;
                break;
            }
            
            break;

            case '3' :
            switch(mode){
                case 1 :
                Main(fd, mode, lcd);
                mode = 0;
                break;

                case 2 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                mode = 5;
                break;

                case 3 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 4 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                pancake_tmp *= 10;
                pancake_tmp += 3;
                Order_Num(fd, lcd, pancake_tmp);
                break;

                case 5 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                blacktea_tmp *= 10;
                blacktea_tmp += 3;
                Order_Num(fd, lcd, blacktea_tmp);
                break;

                case 6 :
                Order(fd, mode, lcd);
                mode = 2;
                break;
            }

            break;

            case '4' :
            switch(mode){
                case 1 :
                Main(fd, mode, lcd);
                mode = 0;
                break;

                case 2 :
                led_cnt = true;
                mode = 6;
                break;

                case 3 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 4 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                pancake_tmp *= 10;
                pancake_tmp += 4;
                Order_Num(fd, lcd, pancake_tmp);
                break;

                case 5 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                blacktea_tmp *= 10;
                blacktea_tmp += 4;
                Order_Num(fd, lcd, blacktea_tmp);
                break;

                case 6 :
                Order(fd, mode, lcd);
                mode = 2;
                break;
            }

            break;

            case '5' :
            switch(mode){
                case 1 :
                Main(fd, mode, lcd);
                mode = 0;
                break;

                case 2 :
                Main(fd, mode, lcd);
                mode = 0;
                pancake_num = 0;
                blacktea_num = 0;
                pancake_tmp = 0;
                blacktea_tmp = 0;
                pancake_done_num = -1;
                blacktea_done_num = -1;
                led_cnt = false;
                timer = -1;
                break;

                case 3 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 4 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                pancake_tmp *= 10;
                pancake_tmp += 5;
                Order_Num(fd, lcd, pancake_tmp);
                break;

                case 5 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                blacktea_tmp *= 10;
                blacktea_tmp += 5;
                Order_Num(fd, lcd, blacktea_tmp);
                break;

                case 6 :
                Order(fd, mode, lcd);
                mode = 2;
                break;
            }

            break;

            case '#' :
            switch (mode){
                case 1 :
                case 2:
                Main(fd, mode, lcd);
                mode = 0;
                break;

                case 3 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 4 :
                case 5 :
                Order(fd, mode, lcd);
                mode = 2;
                pancake_num += pancake_tmp;
                blacktea_num += blacktea_num;
                pancake_tmp = 0;
                blacktea_num = 0;
                break;

                case 6 :
                Order(fd, mode, lcd);
                mode = 2;
                break;
            }

            break;

            default :
            switch(mode){
                case 1 :
                case 2 :
                Main(fd, mode, lcd);
                mode = 0;
                break;

                // case 2 :
                // Main(fd, mode, lcd);
                // mode = 0;
                // break;

                case 3 :
                Order(fd, mode, lcd);
                mode = 2;
                break;

                case 4 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                pancake_tmp *= 10;
                pancake_tmp += (char)key - '0';
                Order_Num(fd, lcd, pancake_tmp);
                break;

                case 5 :
                ioctl(fd, LCD_IOCTL_CLEAR, NULL);
                blacktea_tmp *= 10;
                blacktea_tmp += 5;
                Order_Num(fd, lcd, blacktea_tmp);
                break;

                case 6 :
                Order(fd, mode, lcd);
                mode = 2;
                break;
                
            }

            break;
            
            }

        while(led_cnt)
        {
            timer++;
            printf("timer = %d\n", timer);

            if(timer%5 == 0 && pancake_done_num < pancake_num)
                pancake_done_num++;
            if(timer%3 == 0 && blacktea_done_num < blacktea_num)
                blacktea_done_num++;
            Check_Out(fd, lcd, no_num, pancake_num, blacktea_num, pancake_done_num, blacktea_done_num);

            if(pancake_done_num < pancake_num)
            {
                switch(timer%5)
                {
                    case 0:
                    {
                        led_data = LED_D9_INDEX;
                        break;
                    }
                    case 1:
                    {
                        led_data = LED_D10_INDEX;
                        break;
                    }
                    case 2:
                    {
                        led_data = LED_D11_INDEX;
                        break;
                    }
                    case 3:
                    {
                        led_data = LED_D12_INDEX;
                        break;
                    }
                    case 4:
                    {
                        led_data = LED_D13_INDEX;
                        break;
                    }
                }
                ioctl(fd, LED_IOCTL_BIT_SET, &led_data);
            }

            if(pancake_done_num < blacktea_num)
            {
                switch(timer%3)
                {
                    case 0:
                    {
                        led_data = LED_D14_INDEX;
                        break;
                    }
                    case 1:
                    {
                        led_data = LED_D15_INDEX;
                        break;
                    }
                    case 2:
                    {
                        led_data = LED_D16_INDEX;
                        break;
                    }
                }
                ioctl(fd, LED_IOCTL_BIT_SET, &led_data);
            }

            if(pancake_done_num == pancake_num && blacktea_done_num == blacktea_num)
            {
                Main(fd, mode, lcd);
                display_7seg(fd, _7seg, no_num);
                led_data = LED_ALL_OFF;
                ioctl(fd, LED_IOCTL_SET, &led_data);
                no_num++;
                mode = 0;
                pancake_num = 0;
                blacktea_num = 0;
                pancake_tmp = 0;
                blacktea_tmp = 0;
                pancake_done_num = 0;
                blacktea_done_num = 0;
                led_cnt = false;
                timer = -1;
                break;
            }

            sleep(1);

            if((timer > 0 && timer%5 == 4) || pancake_done_num >= pancake_num)
            {
                led_data = LED_D9_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
                led_data = LED_D10_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
                led_data = LED_D11_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
                led_data = LED_D12_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
                led_data = LED_D13_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
            }

            if((timer > 0 && timer%3 == 2) || blacktea_done_num >= blacktea_num)
            {
                led_data = LED_D14_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
                led_data = LED_D15_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
                led_data = LED_D16_INDEX;
                ioctl(fd, LED_IOCTL_BIT_CLEAR, &led_data);
            }
        }

    }

    close(fd);

    return 0;
}
