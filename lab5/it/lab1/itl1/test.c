#include <stdio.h>

char * foo(int size){
	char a[size];
	unsigned int i = 0;
	for(i=0;i<size;i++)
		a[i] = '!';
	return a;
}

int main()
{
	char * temp;
	temp = foo(10);
	unsigned int i = 0;
	for(i=0;i<10;i++)
		printf("%c",temp[i]);
	return 0;
}