#include <stdio.h>

int len(char filename[]) // ���������� �������� � �����
{
	unsigned int sum = 0;
	int a = 0;
	FILE *op;
	
	op = fopen(filename,"r");
	while(1){
		sum = fgetc(op);
		if (sum == EOF){
			if (feof(op)!=1){ // �������� �� ����� �����
				break;				
			}			
		}
		a++;
		if (sum == 10){ // ���������� ��� tab 
		a++;
		}
	}
	fclose(op);
	return a;
}

void it23(char name[]) // ������� �� iso 8859-5 � windows-1251
{ 	FILE *fo; // ���������� ��������� �� ����� fo
	FILE *fw; // ���������� ��������� �� ����� fw
	int i = 0;	
	unsigned char temp[len(name)]; //������ ��� �������� �������� ������� � �������� �������
	
	fo = fopen(name, "rb"); // �������� ����� � ������ ��������� ������
	fw = fopen("asw.txt", "a"); // �������� ����� � ������ ������ � �����������
	
	fread(&temp, sizeof(char), len(name),fo); // ������ � ������ temp �� ������ fo 

	for (i=0; i<=len(name); i++){ 
		if (temp[i] == 161){ // ������� ��� ����� "�"
			temp[i] = 168;
		}
		if (temp[i] == 241){ // ������� ��� ����� "�"
			temp[i] = 184;
		}
		if (temp[i] > 127){ // ������� ��� ������ 127 ASCII ����������� ��������
			temp[i] = temp[i] + 16;	//�������� �� 16 ��� 		
		}
	}

	fwrite(&temp, sizeof(char), len(name), fw); // ������ �� ������� temp � ����� fw
	fputs("\n\n", fw);

	fclose(fw); // �������� ������
	fclose(fo);
}

void it1(char name[]) // ������� �� oem 866 � windows-1251
{
	FILE *fo;// ���������� ��������� �� ����� f�
	FILE *fw;// ���������� ��������� �� ����� fw
	unsigned char temp[len(name)];//������ ��� �������� �������� ������� � �������� �������
	int i = 0;
	
	fo = fopen(name, "rb");// �������� ����� � ������ ��������� ������
	fw = fopen("asw.txt", "a");// �������� ����� � ������ ������ � �����������
	
	fread(&temp, sizeof(char), len(name), fo);// ������ � ������ temp �� ������ fo
	
	for (i=0; i<=len(name); i++){
		if (temp[i] > 127){// ������� ��� ������ 127 ASCII ����������� ��������
			if (temp[i] < 224){	// ������� �������� �� � �� � (���������) � �� � �� � (��������)	
				temp[i] = temp[i] + 64;
			}// ������� ��� ����� "�"
			/*if (temp[i] == 240){// ������� ��� ����� "�" �������� � ���� ��������� CASE!
			temp[i] = 0xA8;
			}
			if (temp[i] == 241){// ������� ��� ����� "�"
				temp[i] = 0xB8;
			}*/
			
			else temp[i] = temp[i] + 16;// ������� �������� �� � �� � (��������) 			
		}
	}
	fwrite(temp, sizeof(char), len(name), fw); // ������ �� ������� temp � ����� fw
	fputs("\n\n", fw);
	
	fclose(fw);// �������� ������
	fclose(fo);
	
	
}

void mac(char name[]){
	FILE *fo;// ���������� ��������� �� ����� f�
	FILE *fw;// ���������� ��������� �� ����� fw
	unsigned char temp[len(name)];//������ ��� �������� �������� ������� � �������� �������
	int i = 0;
	
	fo = fopen(name, "rb");// �������� ����� � ������ ��������� ������
	fw = fopen("as.txt", "a");// �������� ����� � ������ ������ � �����������
	
	fread(&temp, sizeof(char), len(name), fo);// ������ � ������ temp �� ������ fo
	
	for (i=0; i<=len(name); i++){
		printf("I=%x\n",temp[i]);
		if (temp[i] == 0xDE){
			temp[i] = 0xB8;
		}
		if (temp[i] == 0xDD){
			temp[i] = 0xA8;//
		}
		if (temp[i] == 223){//� 
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
			
	fwrite(temp, sizeof(char), len(name), fw); // ������ �� ������� temp � ����� fw
	fputs("\n\n", fw);
	
	fclose(fw);// �������� ������
	fclose(fo);
}
	
int main() 
{	FILE *wh; // �������� ����� ����� ��������
	FILE *cl;
	cl = fopen("as.txt", "wb");
	wh = fopen("asw.txt", "wb");
	fclose(wh);	
	fclose(cl);
	
	it1("sample 1.txt"); //����� ������� ��� ��������������� ��������
	it23("sample 2.txt");
	it23("sample 3.txt");
	//mac("mac.txt");
	
	return 0;	
}