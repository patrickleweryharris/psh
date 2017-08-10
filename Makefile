FLAGS = -Wall -std=gnu99
DEPENDENCIES = src/pshell.h

all: psh

psh: src/psh.o src/worker.o src/command_parser.o src/special_functions.o
	gcc ${FLAGS} -o $@ $^

src/%.o: %.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

clean:
	rm src/*.o psh

run: psh
	./psh
