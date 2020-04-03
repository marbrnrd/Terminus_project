all : shell

shell : shell.o
	gcc -o shell.o

shell.o : shell.c
	gcc -o shell.o -c shell.c -Wall -O

clean: 
	rm -f *.o core

mrproper: clean
	rm -f shell
