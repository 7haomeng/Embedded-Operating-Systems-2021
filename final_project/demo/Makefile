CC = gcc -pthread
CXX = g++ -pthread
FLAGS = -g -Wall

all: server client 

%.o: %.c
	$(CC) -c $< $(FLAGS)

%.o: %.cpp
	$(CXX) -c $< $(FLAGS)

server: server.o
	$(CXX) -o $@ $^  $(FLAGS)
	
client: client.o
	$(CXX) -o $@ $^  $(FLAGS)

clean:
	rm -rf server server.o
	rm -rf client client.o

.PHONY: all clean