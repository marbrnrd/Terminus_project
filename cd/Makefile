#Makefile

all: main

main: main.o cd.o
	gcc main.o cd.o -Wall -O

print_page.o: print_page.c
	gcc -o cd.o -c cd.c -Wall -O

main.o: main.c cd.h
	gcc -o main.o -c main.c -Wall -O

clean:
	rm -rf *.o core

mrproper: clean
	rm -rf main

