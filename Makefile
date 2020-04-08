all : shell

shell : shell.o cd.o
	gcc -o shell.o

shell.o : shell.c cd.h
	gcc -o shell.o -c shell.c -Wall -O

cd.o : cd/cd.c
	fcc -o cd.o -c cd.c -Wall -O

clean: 
	rm -f *.o core

mrproper: clean
	rm -f shell
