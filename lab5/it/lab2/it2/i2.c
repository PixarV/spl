#include <stdio.h>

int len(char filename[], int acc) // количество символов в файле
{
	unsigned int sum = 0;
	int a = 0;
	int b = 0;
	FILE *op;
	
	op = fopen(filename,"r");
	while(1){
		sum = fgetc(op);
		if (sum == EOF){
			if (feof(op)!=1){ // Проверка на конец файла
				break;				
			}			
		}
		if (sum <= 0xA0){
			//printf("%x\n", sum);
			b++;
		}
		a++;
		if (sum == 10){ // исключение для tab 
		a++;
		}
	}
	fclose(op);
	switch(acc){
	case 1: return a;
	case 2: return b;
	}

}

void isoutf(char file[]){
	FILE *fp;
	FILE *fw;
	int i;
	int ln1, ln2;
	ln1 = len(file, 1);
	ln2 = len(file, 2);
	int e = 0;
	int c = 0;
	int mrac = 0;	
	unsigned char temp[ln1];
	unsigned short tempout[ln2];
	
	fp = fopen(file , "rb");
	fw = fopen("s2.txt" , "wb");

	fread(&temp, sizeof(char), ln1, fp);
	for (e = 0; e<=ln2+12;e++){
		for(i = 0; i<=ln2; i++){
			if (temp[c] <= 0xA0){
				fwrite(tempout, 2, i, fw);
				fseek(fw, 2*c - mrac, SEEK_SET);
				putc(temp[c], fw);
				mrac++;
				c++;				
				break;
			}
			switch (temp[c]){
			case 0xA1:
				tempout[i] = 0x81d0;
				break;
			case 0xf1:
				tempout[i] = 0x91d1;
				break;
			}
			switch (temp[c]/16){		
			case 0xB:
				tempout[i] = 0x9000 + ((temp[c]%16)*0x100) + 0xd0;
				break;
			case 0xC:
				tempout[i] = 0xA000 + ((temp[c]%16)*0x100) + 0xd0;
				break;
			case 0xD:
				tempout[i] = 0xB000 + ((temp[c]%16)*0x100) + 0xd0;
				break;
			case 0xE:
				tempout[i] = 0x8000 + ((temp[c]%16)*0x100) + 0xd1;
				break;
			}
			c++;
		}
	}
	
	fclose(fw);
	fclose(fp);
	//return 0;
}

void utfiso(char file[]){
	FILE *fp, *fw;
	int i = 0;
	int e = 0;
	int ln1, ln2;
	ln1 = len(file, 1);
	ln2 = len(file, 2);
	
	unsigned char temp[ln1];
	unsigned char tempout[ln1];
	
	fp = fopen(file, "rb");
	fw = fopen("s22.txt", "wb");
	fread(&temp, 1, ln1, fp);

	for (i=0; i<=ln1; i++){
		if (temp[i] <= 0xA0){			
			tempout[e] = temp[i];			
		}
		if (temp[i]/16 == 0xd){				
			i++;			
		}

		switch (temp[i]/16){
			case 0x9:
				tempout[e] = 0xB0 + (temp[i]%16);
				break;
			case 0xA:
				tempout[e] = 0xC0 + (temp[i]%16);
				break;
			case 0xB:
				tempout[e] = 0xD0 + (temp[i]%16);
				break;
			case 0x8:
				tempout[e] = 0xE0 + (temp[i]%16);
				break;
			
			}

		switch (temp[i]){
		case 0x91:
			tempout[e] = 0xf1;
			break;
		case 0x81:
			tempout[e] = 0xE1;
			break;
		//case 0x
		}
		e++;
	}

	//361 s21
	//370 s3
	//378 s2
	fwrite(tempout, 1, 370, fw);
	fclose(fp);
	fclose(fw);
}


int main(){
	isoutf("sample 3.txt");
	//isoutf("sample 2.txt");
	//isoutf("sample 21.txt");
	
	//utfiso("s2.txt");
	return 0;

}
	