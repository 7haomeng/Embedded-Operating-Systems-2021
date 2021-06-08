CC = arm-unknown-linux-gnu-gcc
PATHA = /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/lib/
PATHB = /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/include/
PATHC = /home/tsai/microtime/linux/include/

all:
	${CC} client_pxa.c -o client_pxa -L $(PATHA) -I $(PATHB) -I $(PATHC) -lpthread 


clean:
	rm -rf client_pxa
