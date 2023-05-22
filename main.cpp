#include"graph.h"

int main()
{
	Graph map;
	InitMap(&map);
	int choice;
	do {
		printf("\n**********交通咨询系统**********\n");
		printf("1.添加城市\n");
		printf("2.添加火车路线\n");
		printf("3.添加飞机路线\n");
		printf("4.购买火车票\n");
		printf("5.购买飞机票\n");
		printf("6.用文件导入路线\n");
		printf("7.退出\n");
		printf("*********************************\n");
		printf("请输入你的选择：");
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
			printf("无效的输入，请重新输入\n");
		}
	} while (choice == 0);
	return 0;
}