CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: exe/prog exe/ex2 exe/ex3 #exe/ex4 exe/ex5

obj/Exercice2.o: src/Exercice2.c
	$(CC) $(CFLAGS) -c $<

exe/ex2: obj/Exercice2.o
	$(CC) $(CFLAGS) -o $@ $^

obj/Exercice3.o: src/Exercice3.c
	$(CC) $(CFLAGS) -c $< -o $@

exe/ex3: obj/Exercice3.o obj/main.o obj/Exercice2.o
	$(CC) $(CFLAGS)  -o $@ $^

exe/prog: obj/main.o 
	$(CC) $(CFLAGS) -o $@ $^

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c $<

obj/Exercice4.o: src/Exercice4.c obj/Exercice3.o obj/Exercice2.o obj/main.o
	$(CC) $(CFLAGS) -c $< -o $@

exe/ex4: obj/Exercice4.o
	$(CC) $(CFLAGS) -o $@ $^

obj/Exercice5.o: src/Exercice5.c
	$(CC) $(CFLAGS) -c $< -o $@

exe/ex5: obj/Exercice5.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o exe/prog exe/ex2 exe/ex3 exe/ex4 exe/ex5
