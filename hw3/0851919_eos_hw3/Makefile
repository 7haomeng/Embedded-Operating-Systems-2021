CC = gcc -pthread
CXX = g++ -pthread
FLAGS = -g -Wall

all: hw3 client

%.o: %.c
	$(CC) -c $< $(FLAGS)

%.o: %.cpp
	$(CXX) -c $< $(FLAGS)

hw3: hw3.o
	$(CXX) -o $@ $^  $(FLAGS)

client: client.o
	$(CXX) -o $@ $^  $(FLAGS)

clean:
	rm -rf hw2 hw2.o
	rm -rf client client.o

.PHONY: all clean
