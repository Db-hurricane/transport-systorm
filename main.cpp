#include"graph.h"

int main()
{
	Graph map;
	InitMap(&map);
	int choice;
	do {
		printf("\n**********��ͨ��ѯϵͳ**********\n");
		printf("1.��ӳ���\n");
		printf("2.��ӻ�·��\n");
		printf("3.��ӷɻ�·��\n");
		printf("4.�����Ʊ\n");
		printf("5.����ɻ�Ʊ\n");
		printf("6.���ļ�����·��\n");
		printf("7.�˳�\n");
		printf("*********************************\n");
		printf("���������ѡ��");
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		default:
			printf("��Ч�����룬����������\n");
		}
	} while (choice == 0);
	return 0;
}