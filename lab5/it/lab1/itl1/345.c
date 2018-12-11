#include <stdio.h>

int len(char filename[]) // количество символов в файле
{
	unsigned int sum = 0;
	int a = 0;
	FILE *op;
	
	op = fopen(filename,"r");
	while(1){
		sum = fgetc(op);
		if (sum == EOF){
			if (feof(op)!=1){ // Проверка на конец файла
				break;				
			}			
		}
		a++;
		if (sum == 10){ // исключение для tab 
		a++;
		}
	}
	fclose(op);
	return a;
}

void it23(char name[]) // перевод из iso 8859-5 в windows-1251
{ 	FILE *fo; // объявление указателя на поток fo
	FILE *fw; // объявление указателя на поток fw
	int i = 0;	
	unsigned char temp[len(name)]; //Массив для хранения значения символа в байтовом формате
	
	fo = fopen(name, "rb"); // открытие файла в режиме двоичного чтения
	fw = fopen("asw.txt", "a"); // открытие файла в режиме записи с дополнением
	
	fread(&temp, sizeof(char), len(name),fo); // запись в массив temp из потока fo 

	for (i=0; i<=len(name); i++){ 
		if (temp[i] == 161){ // условие для буквы "Ё"
			temp[i] = 168;
		}
		if (temp[i] == 241){ // условие для буквы "ё"
			temp[i] = 184;
		}
		if (temp[i] > 127){ // условие для первых 127 ASCII совместимых символов
			temp[i] = temp[i] + 16;	//смещение на 16 бит 		
		}
	}

	fwrite(&temp, sizeof(char), len(name), fw); // запись из массива temp в поток fw
	fputs("\n\n", fw);

	fclose(fw); // закрытие файлов
	fclose(fo);
}

void it1(char name[]) // перевод из oem 866 в windows-1251
{
	FILE *fo;// объявление указателя на поток fо
	FILE *fw;// объявление указателя на поток fw
	unsigned char temp[len(name)];//Массив для хранения значения символа в байтовом формате
	int i = 0;
	
	fo = fopen(name, "rb");// открытие файла в режиме двоичного чтения
	fw = fopen("asw.txt", "a");// открытие файла в режиме записи с дополнением
	
	fread(&temp, sizeof(char), len(name), fo);// запись в массив temp из потока fo
	
	for (i=0; i<=len(name); i++){
		if (temp[i] > 127){// условие для первых 127 ASCII совместимых символов
			if (temp[i] < 224){	// перевод символов от А до Я (заглавные) и от а до п (строчные)	
				temp[i] = temp[i] + 64;
			}// условие для буквы "Ё"
			/*if (temp[i] == 240){// условие для буквы "Ё" НАПИСАТЬ В ВИДЕ СТРУКТУРЫ CASE!
			temp[i] = 0xA8;
			}
			if (temp[i] == 241){// условие для буквы "ё"
				temp[i] = 0xB8;
			}*/
			
			else temp[i] = temp[i] + 16;// перевод символов от р до я (строчные) 			
		}
	}
	fwrite(temp, sizeof(char), len(name), fw); // запись из массива temp в поток fw
	fputs("\n\n", fw);
	
	fclose(fw);// закрытие файлов
	fclose(fo);
	
	
}

void mac(char name[]){
	FILE *fo;// объявление указателя на поток fо
	FILE *fw;// объявление указателя на поток fw
	unsigned char temp[len(name)];//Массив для хранения значения символа в байтовом формате
	int i = 0;
	
	fo = fopen(name, "rb");// открытие файла в режиме двоичного чтения
	fw = fopen("as.txt", "a");// открытие файла в режиме записи с дополнением
	
	fread(&temp, sizeof(char), len(name), fo);// запись в массив temp из потока fo
	
	for (i=0; i<=len(name); i++){
		printf("I=%x\n",temp[i]);
		if (temp[i] == 0xDE){
			temp[i] = 0xB8;
		}
		if (temp[i] == 0xDD){
			temp[i] = 0xA8;//
		}
		if (temp[i] == 223){//я 
			temp[i] = 255;
		} 
		
		else {
		if (temp[i] > 127){
			if (224<= temp[i]){
				if (temp[i] <= 254);{
					temp[i]=temp[i];
				}
			}
			if (temp[i] < 224){
				temp[i] = temp[i] + 64;
				
			}
		}
		}
			
		
	}
			
	fwrite(temp, sizeof(char), len(name), fw); // запись из массива temp в поток fw
	fputs("\n\n", fw);
	
	fclose(fw);// закрытие файлов
	fclose(fo);
}
	
int main() 
{	FILE *wh; // отчистка файла перед открытие
	FILE *cl;
	cl = fopen("as.txt", "wb");
	wh = fopen("asw.txt", "wb");
	fclose(wh);	
	fclose(cl);
	
	it1("sample 1.txt"); //вызов функций для соответствующих отрывков
	it23("sample 2.txt");
	it23("sample 3.txt");
	//mac("mac.txt");
	
	return 0;	
}