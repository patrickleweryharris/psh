FLAGS = -Wall -std=gnu99 -g
DEPENDENCIES = psh.h

all: psh

psh: psh.o shell.o
	gcc ${FLAGS} -o $@ $^

%.o: %.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

clean:
	rm *.o psh
