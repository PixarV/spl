ASM = nasm
AFLAGS = -felf64

all: load 


load: main.asm lib.asm dict.asm
	ld main.o lib.o dict.o -o main

main.o: main.o
	$(ASM) $(AFLAGS) main.asm

lib.o:  lib.o
	$(ASM) $(AFLAGS) lib.asm

dict.o: dict.o
	$(ASM) $(AFLAGS) dict.asm

clean: 
	rm main lib dict 
