all: exe/main1 exe/main2 exe/main3 exe/main4 exe/main5 exe/main6 exe/main7 exe/main8

exe/main1: obj/main.o obj/main1.o
	gcc -o $@ $^
obj/main1.o: src/main1.c
	gcc -W -c $^
obj/main.o: src/main.c
	gcc -W -c $^ -o $@


exe/main2: obj/Exercice2.o obj/main2.o
	gcc -o $@ $^
obj/main2.o: src/main2.c
	gcc -W -c $^
obj/Exercice2.o: src/Exercice2.c
	gcc -W -c $^ -o $@
	
exe/main3: obj/Exercice3.o obj/Exercice2.o obj/main.o
	gcc -o $@ $^
obj/main3.o: src/main3.c
	gcc -W -c $^ -o $@
obj/Exercice3.o: src/Exercice3.c
	gcc -W -c $^ -o $@

exe/main4: obj/Exercice4.o obj/Exercice3.o obj/Exercice2.o obj/main.o 
	gcc -o $@ $^
obj/main4.o: src/main4.c
	gcc -W -c $^ -o $@
obj/Exercice4.o: src/Exercice4.c
	gcc -W -c $^ -o $@

exe/main5: obj/Exercice5.o obj/Exercice4.o obj/Exercice3.o obj/Exercice2.o obj/main.o 
	gcc -o $@ $^
obj/main5.o: src/main5.c
	gcc -W -c $^ -o $@
obj/Exercice5.o: src/Exercice5.c
	gcc -W -c $^ -o $@

exe/main6: obj/Exercice6.o obj/Exercice5.o obj/Exercice4.o obj/Exercice3.o obj/Exercice2.o obj/main.o  
	gcc -o $@ $^
obj/main6.o: src/main6.c
	gcc -W -c $^ -o $@
obj/Exercice6.o: src/Exercice6.c
	gcc -W -c $^ -o $@


exe/main7: obj/Exercice7.o obj/Exercice6.o obj/Exercice5.o obj/Exercice4.o obj/Exercice3.o obj/Exercice2.o obj/main.o  
	gcc -o $@ $^
obj/main7.o: src/main7.c
	gcc -W -c $^ -o $@
obj/Exercice7.o: src/Exercice7.c
	gcc -W -c $^ -o $@

exe/main8: obj/Exercice8.o obj/Exercice7.o obj/Exercice6.o obj/Exercice5.o obj/Exercice4.o obj/Exercice3.o obj/Exercice2.o obj/main.o  
	gcc -o $@ $^
obj/main8.o: src/main8.c
	gcc -W -c $^ -o $@
obj/Exercice8.o: src/Exercice8.c
	gcc -W -c $^ -o $@

exe/myGit: obj/myGit.o obj/Exercice8.o obj/Exercice7.o obj/Exercice6.o obj/Exercice5.o obj/Exercice4.o obj/Exercice3.o obj/Exercice2.o obj/main.o  
	gcc -o $@ $^
obj/myGit.o: src/myGit.c
	gcc -W -c $^ -o $@

clean:
	rm -f obj/*.o exe/*.exe
