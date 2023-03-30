CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: exe/prog exe/ex2 exe/ex3

obj/Exercice2.o: src/Exercice2.c
	$(CC) $(CFLAGS) -c $< -o $@

exe/ex2: obj/Exercice2.o
	$(CC) $(CFLAGS) -o $@ $^

obj/Exercice3.o: src/Exercice3.c
	$(CC) $(CFLAGS) -c $< -o $@

exe/ex3: obj/Exercice3.o obj/main.o
	$(CC) $(CFLAGS) -o $@ $^

exe/prog: obj/main.o
	$(CC) $(CFLAGS) -o $@ $^

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o exe/prog exe/ex2 exe/ex3