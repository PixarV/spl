#include <stdio.h>
#include <sys/mman.h>

extern int sum(int, int);

int main() {
	FILE* obj_file = fopen("sum.o", "r");

	fseek(obj_file, 0, SEEK_END);
	long size = ftell(obj_file);	
	printf("Size: %d\n", size);

	void* block =  mmap(NULL, 
					 	size, 
					 	PROT_READ | PROT_WRITE | PROT_EXEC, 
					 	MAP_PRIVATE,  
					 	fileno(obj_file), 0);

	printf("Block addr: %x\n", block);
	int first = 5;
	int second = 6;
	int result = 0;

	asm("mov rax, QWORD PTR[rbp-0x8]"); // block
	asm("mov edi, DWORD PTR[rbp-0x24]"); // first
	asm("mov esi, DWORD PTR[rbp-0x20]"); // second
	asm("add rax, 0x40"); // offcet in sum.o
	asm("call rax"); // sum
	asm("mov DWORD PTR[rbp-0x1c], eax"); // result
	
	printf("Res: %d\n", result);

	fclose(obj_file);
}
