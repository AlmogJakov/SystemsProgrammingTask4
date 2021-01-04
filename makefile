CC=gcc
FLAGS= -Wall -g

all: frequency
frequency: frequency.o
	$(CC) $(FLAGS) -o frequency frequency.o
frequency.o: frequency.c
	$(CC) $(FLAGS) -c frequency.c
.PHONY: clean all
clean:
	rm -f *.o frequency