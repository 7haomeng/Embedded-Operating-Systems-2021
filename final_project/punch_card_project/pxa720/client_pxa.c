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
#include "/home/tsai/microtime/linux/include/asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include <sys/fcntl.h>
#include <sys/ioctl.h>

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

void lcd_show(int fd, int num){
    
    lcd_write_info_t display;

    switch(num){
        case 0:
            display.Count = sprintf((char * ) display.Msg, "0");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 1:
            display.Count = sprintf((char * ) display.Msg, "1");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 2:
            display.Count = sprintf((char * ) display.Msg, "2");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 3:
            display.Count = sprintf((char * ) display.Msg, "3");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 4:
            display.Count = sprintf((char * ) display.Msg, "4");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 5:
            display.Count = sprintf((char * ) display.Msg, "5");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 6:
            display.Count = sprintf((char * ) display.Msg, "6");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 7:
            display.Count = sprintf((char * ) display.Msg, "7");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        case 8:
            display.Count = sprintf((char * ) display.Msg, "8");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;
        default:
            display.Count = sprintf((char * ) display.Msg, "9");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
            break;

    }
}


void seven_seg1(int fd,int num){
    _7seg_info_t data;
	
	data.Mode = _7SEG_MODE_HEX_VALUE;
	data.Which = _7SEG_D5_INDEX;
	
	switch(num){
		case 0:
			data.Value = 0x0;
			break;
		case 1:
			data.Value = 0x1;
			break;
		case 2:
			data.Value = 0x2;
			break;
		case 3:
			data.Value = 0x3;
			break;
		case 4:
			data.Value = 0x4;
			break;
		case 5:
			data.Value = 0x5;
			break;
		case 6:
			data.Value = 0x6;
			break;
		case 7:
			data.Value = 0x7;
			break;
		case 8:
			data.Value = 0x8;
			break;
		default:
			data.Value = 0x9;
			break;
	}
	ioctl(fd, _7SEG_IOCTL_SET, &data);
}
void seven_seg2(int fd,int num){
    _7seg_info_t data;
	
	data.Mode = _7SEG_MODE_HEX_VALUE;
	data.Which = _7SEG_D6_INDEX;
	
	switch(num){
		case 0:
			data.Value = 0x0;
			break;
		case 1:
			data.Value = 0x1;
			break;
		case 2:
			data.Value = 0x2;
			break;
		case 3:
			data.Value = 0x3;
			break;
		case 4:
			data.Value = 0x4;
			break;
		case 5:
			data.Value = 0x5;
			break;
		case 6:
			data.Value = 0x6;
			break;
		case 7:
			data.Value = 0x7;
			break;
		case 8:
			data.Value = 0x8;
			break;
		default:
			data.Value = 0x9;
			break;
	}
	ioctl(fd, _7SEG_IOCTL_SET, &data);
}
void seven_seg3(int fd, int num){
    _7seg_info_t data;
	
	data.Mode = _7SEG_MODE_HEX_VALUE;
	data.Which = _7SEG_D7_INDEX;
	
	switch(num){
		case 0:
			data.Value = 0x0;
			break;
		case 1:
			data.Value = 0x1;
			break;
		case 2:
			data.Value = 0x2;
			break;
		case 3:
			data.Value = 0x3;
			break;
		case 4:
			data.Value = 0x4;
			break;
		case 5:
			data.Value = 0x5;
			break;
		case 6:
			data.Value = 0x6;
			break;
		case 7:
			data.Value = 0x7;
			break;
		case 8: 
			data.Value = 0x8;
			break;
		default:
			data.Value = 0x9;
			break;
	}
	ioctl(fd, _7SEG_IOCTL_SET, &data);
}
void seven_seg4(int fd,int num){
    _7seg_info_t data;
	
	data.Mode = _7SEG_MODE_HEX_VALUE;
	data.Which = _7SEG_D8_INDEX;
	
	switch(num){
		case 0:
			data.Value = 0x0;
			break;
		case 1:
			data.Value = 0x1;
			break;
		case 2:
			data.Value = 0x2;
			break;
		case 3:
			data.Value = 0x3;
			break;
		case 4:
			data.Value = 0x4;
			break;
		case 5:
			data.Value = 0x5;
			break;
		case 6:
			data.Value = 0x6;
			break;
		case 7:
			data.Value = 0x7;
			break;
		case 8:
			data.Value = 0x8;
			break;
		default:
			data.Value = 0x9;
			break;
	}
	ioctl(fd, _7SEG_IOCTL_SET, &data);
}

void ledgreen(int fd){
    unsigned short data;

    data = LED_D9_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D11_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D13_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D15_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    sleep(1);

    data = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &data);
    sleep(2);

    data = LED_D9_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D11_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D13_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D15_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    sleep(1);

    data = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &data);
}
void ledred(int fd){
    unsigned short data;


    data = LED_D10_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D12_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D14_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D16_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    sleep(1);

    data = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &data);
    sleep(2);

    data = LED_D10_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D12_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D14_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    data = LED_D16_INDEX;
    ioctl(fd, LED_IOCTL_BIT_SET, &data);
    sleep(1);

    data = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &data);

}

int people=0, date=0;

int main(int argc, char *argv[])
{

    /*socket*/
    int connfd;
    int n, fd;
    char snd[BUFSIZE], buf[BUFSIZE];
    lcd_write_info_t display;
    unsigned short data;

    if (argc != 3)
    {
        printf("Ussage: %s host_address host_port\n", argv[0]);
    }

    connfd = connectsock(argv[1], argv[2], "tcp");

    // pxa device check
    if((fd = open("/dev/lcd", O_RDWR)) < 0)  return (-1);

    //start the punch
    unsigned short key;
    int ret;

    seven_seg1(fd, 0);
    seven_seg2(fd, 0);
    seven_seg3(fd, 0);
    seven_seg4(fd, 0);

    data = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &data);

    while (1)
    {
        ioctl( fd, LCD_IOCTL_CLEAR, NULL);

        display.Count = sprintf((char * ) display.Msg, "Input ID: ");
        ioctl(fd, LCD_IOCTL_WRITE, &display);

        int times=2;
        int id=0;
        char s[2];
        while(times){
            ret = ioctl(fd, KEY_IOCTL_CHECK_EMTPY, &key);
            if(ret < 0){
                sleep(1);
                continue;
            }

            ret = ioctl(fd, KEY_IOCTL_GET_CHAR, &key);

            if((key & 0xff) == '1'){
                s[2-times] = '1';
                display.Count = sprintf((char * ) display.Msg, "1");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '2'){
                s[2-times] = '2';
                display.Count = sprintf((char * ) display.Msg, "2");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '3'){
               s[2-times] = '3';
                display.Count = sprintf((char * ) display.Msg, "3");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '4'){
               s[2-times] = '4';
                display.Count = sprintf((char * ) display.Msg, "4");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '5'){
               s[2-times] = '5';
                display.Count = sprintf((char * ) display.Msg, "5");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '6'){
               s[2-times] = '6';
                display.Count = sprintf((char * ) display.Msg, "6");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '7'){
               s[2-times] = '7';
                display.Count = sprintf((char * ) display.Msg, "7");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '8'){
               s[2-times] = '8';
                display.Count = sprintf((char * ) display.Msg, "8");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else if((key & 0xff) == '9'){
               s[2-times] = '9';
                display.Count = sprintf((char * ) display.Msg, "9");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            else{
               s[2-times] = '0';
                display.Count = sprintf((char * ) display.Msg, "0");
                ioctl(fd, LCD_IOCTL_WRITE, &display);
            }
            times--;
        }

        id = (s[0]-48)*10 + (s[1]-48);


        memset(snd, 0, BUFSIZE); //clean the string
        n = sprintf(snd, "%d", id);
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
        if(info[1] != date){
            date = info[1];
            people=0;
            seven_seg4(fd,0);
        }
        //根據info[8]印出相應的內容在terminal上
        if (info[4] == 1)
        { //under clean
            display.Count = sprintf((char * ) display.Msg, "\nblocked");
            ioctl(fd, LCD_IOCTL_WRITE, &display);
                    

            if (info[6] == 0 && info[5] == 1)
            {
                display.Count = sprintf((char * ) display.Msg, "\npounched");
                ioctl(fd, LCD_IOCTL_WRITE, &display);

                ledred(fd);
            }
            else if (info[6] != 0)
            { //check in fail
                display.Count = sprintf((char * ) display.Msg, "\nisolated ");
                ioctl(fd, LCD_IOCTL_WRITE, &display);

                lcd_show(fd, info[7]);

                ledred(fd);
        
            }

        }
        else if (info[6] == 0 && info[3] != 5 && info[5] == 0)
        { //check in success
            display.Count = sprintf((char * ) display.Msg, "\nsuccessed");
            ioctl(fd, LCD_IOCTL_WRITE, &display);

            people++;

            seven_seg1(fd, 0);
            seven_seg2(fd, 0);
            seven_seg3(fd, 0);
            seven_seg4(fd, people);

            ledgreen(fd);

        }
        else if (info[6] == 0 && info[5] == 1)
        {
            display.Count = sprintf((char * ) display.Msg, "\nfailed punched");
            ioctl(fd, LCD_IOCTL_WRITE, &display);

            ledred(fd);
        }
        else if (info[6] == 0 && info[3] == 5)
        {
            display.Count = sprintf((char * ) display.Msg, "\nfailed too \nmany people");
            ioctl(fd, LCD_IOCTL_WRITE, &display);

            ledred(fd);
        }
        else if (info[6] != 0)
        { //check in fail
            display.Count = sprintf((char * ) display.Msg, "\nfailed isolated ");
            ioctl(fd, LCD_IOCTL_WRITE, &display);

            lcd_show(fd, info[7]);

            ledred(fd);
        }

    	//sleep(5);
    }

    close(connfd);

    return 0;
}

