CC = gcc
CXX = g++
FLAGS = -g -Wall

all: game guess

%.o: %.c
	$(CC) -c $< $(FLAGS)

%.o: %.cpp
	$(CXX) -c $< $(FLAGS)

game: game.o
	$(CXX) -o $@ $^  $(FLAGS)

guess: guess.o
	$(CXX) -o $@ $^  $(FLAGS)

clean:
	rm -rf game game.o
	rm -rf guess guess.o

.PHONY: all clean