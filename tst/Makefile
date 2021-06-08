CC = gcc
CXX = g++
FLAGS = -g -Wall

all: tst

%.o: %.c
	$(CC) -c $< $(FLAGS)

%.o: %.cpp
	$(CXX) -c $< $(FLAGS)

tst: tst.o
	$(CXX) -o $@ $^ $(FLAGS)

clean:
	rm -rf tst tst.o

.PHONY: all clean
