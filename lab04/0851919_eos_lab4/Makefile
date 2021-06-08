CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++
FLAGS = -g -Wall --static

DIR = /home/chinghaomeng/linux-xlnx
obj-m = mydev.o

all: mydev writer reader

mydev:
	make -C $(DIR) M=$(PWD) modules

%.o: %.cpp
	$(CXX) -c $< $(FLAGS)

writer: writer.o
	$(CXX) -o $@ $^ $(FLAGS)

reader: reader.o
	$(CXX) -o $@ $^ $(FLAGS)

clean:
	make -C $(DIR) M=$(PWD) clean
	rm -rf writer writer.o
	rm -rf reader reader.o

.PYONY: all clean
