CC = arm-linux-gnueabihf-gcc
FLAGS = -g -Wall --static

all: server_ZC702 client_ZC702

server_ZC702: server_ZC702.c server_vec.h
	$(CC) -o $@ server_vec.c server_ZC702.c -lpthread $(FLAGS)

client_ZC702: client_ZC702.c
	gcc -o $@ $^ $(FLAGS)

clean:
	rm -rf server_ZC702 server_ZC702.o server_vec.o
	rm -rf client_ZC702 client_ZC702.o

.PYONY: all clean