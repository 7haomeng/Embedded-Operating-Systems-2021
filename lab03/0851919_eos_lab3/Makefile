CC = arm-unknown-linux-gnu-gcc
CXX = arm-unknown-linux-gnu-g++
CFLAGS = -g -Wall

LINK = /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/lib/
INCLUDE1 = /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/include/
INCLUDE2 = /home/chinghaomeng/PXA270-20210303/microtime/linux/include/

all: lab3

%.o: %.c
	$(CC) -c $< -L $(LINK) -I $(INCLUDE1) -I $(INCLUDE2) $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -L $(LINK) -I $(INCLUDE1) -I $(INCLUDE2) $(CFLAGS)

lab3: lab3.o
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm -rf lab3 lab3.o

.PHONY: all clean
