all: application

application: simple_ll.o
	gcc main.c simple_ll.o -o lab -lm

simple_ll.o: simple_ll.c
	gcc -c simple_ll.c

clean:
	rm simple_ll.o lab 
