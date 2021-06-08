CC = gcc -pthread
CXX = g++ -pthread
FLAGS = -g -Wall

all: hw2 

%.o: %.c
	$(CC) -c $< $(FLAGS)

%.o: %.cpp
	$(CXX) -c $< $(FLAGS)

hw2: hw2.o
	$(CXX) -o $@ $^  $(FLAGS)

clean:
	rm -rf hw2 hw2.o

.PHONY: all clean
