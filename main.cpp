#include <string>
#include "RSA_frost0705.h"
#include <iostream>

using namespace std;
void main()
{
	FILE *file;

	//int c[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	char *path_ek = "C:\\Users\\Frost0705\\Desktop\\encryption_key.txt";
	char *path_dk = "C:\\Users\\Frost0705\\Desktop\\decryption_key.txt";
	char *path_ot = "C:\\Users\\Frost0705\\Desktop\\open_text.txt";
	char *path_et = "C:\\Users\\Frost0705\\Desktop\\encrypted_text.txt";
	char *path_dt = "C:\\Users\\Frost0705\\Desktop\\decryption_key.txt";
	
	cBigNumber d,e,q,p,n;
	/*
	fopen_s(&file,path_ot,"wb");
	int temp = sizeof(c);
	//���� ������� �� ������ ������� �������, ����� �������������
	fwrite(&temp,sizeof(int),1,file);
	fwrite(&c,temp,1,file);
	fclose(file);
	*/
	//���������� ������. �� ������ ������ ����������� ������ �����(��������� ������� �����). ������� ������, ������� ���������� ������� ����� � ������� �� � ����
	preparation(&e,&d,&p,&q,&n,30);
	//��� �������, ����������� ��� ����������/�������������
	int *open, *encrypted, *decrypted;
	//�������� ������ ���� ������
	FILE *file_begin, *file_end;
	fopen_s(&file_begin,path_ot,"r");
	fopen_s(&file_end,path_et,"w");
	//���������� �������� ������
	int sizeOfMassege;
	fread(&sizeOfMassege,sizeof(int),1,file_begin);
	open = new int[sizeOfMassege];
	fread(open,sizeOfMassege,1,file_begin);
	//��������� ������ ��� ������������� ����
	encrypted = new int[sizeOfMassege];
	int a = (sizeOfMassege/(sizeof(int)));
	//���������� �����
	for (int i = 0; i < a; i++)
	{
		encrypted[i] = EnDeCryption(e,n,open[i]);
		a = a;
	}
	//������ ����� � ����� ����
	fwrite(&sizeOfMassege, sizeof(int),1,file_end);
	fwrite(encrypted, sizeOfMassege,1,file_end);
	//��������	
	fclose(file_begin);
	fclose(file_end);

	//�������� ����� ���� ������
	fopen_s(&file_begin,path_et,"r");
	fopen_s(&file_end,path_dt,"w");
	//���������� �������������� �����
	fread(&sizeOfMassege,sizeof(int),1,file_begin);
	encrypted = new int[sizeOfMassege];
	fread(encrypted,sizeOfMassege,1,file_begin);
	//��������� ������ ��� ����� ����
	decrypted = new int[sizeOfMassege];

	a = (sizeOfMassege/(sizeof(int)));
	//�������������
	for (int i = 0; i < a; i++)
	{
		decrypted[i] = EnDeCryption(d,n,encrypted[i]);
		a = a;
	}
	//������
	fwrite(&sizeOfMassege, sizeof(int),1,file_end);
	fwrite(decrypted, sizeOfMassege,1,file_end);
	//��������
	fclose(file_begin);
	fclose(file_end);
	
	_getch();

}