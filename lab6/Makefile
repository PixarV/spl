all: application

application: sum.o
	gcc -g -masm=intel exec.c

sum.o:
	gcc -c sum.c -o sum.o

clean:
	rm sum.o a.out
