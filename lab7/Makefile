all: application

application: hard.o 
	gcc -g -no-pie bitmap.c main.c hard.o -o lab

hard.o: hard.asm
	nasm -g -felf64 hard.asm -o hard.o

clean:
	rm *.o lab hhh.bmp
