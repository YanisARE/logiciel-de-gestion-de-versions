CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: prog ex2 ex3

Exercice2.o: Exercice2.c
	$(CC) $(CFLAGS) -c $<

ex2: Exercice2.o
	$(CC) $(CFLAGS) -o $@ $^

Exercice3.o: Exercice3.c
	$(CC) $(CFLAGS) -c $<

ex3: Exercice3.o main.o
	$(CC) $(CFLAGS)  -o $@ Exercice3.o

prog: main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o prog ex2 ex3
