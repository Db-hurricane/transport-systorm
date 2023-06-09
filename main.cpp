#include"graph.h"
#include <time.h>

void initMap(Map* map) {
	// ���������б�
	const char* cityNames[15] = { "����", "�Ϻ�", "����", "����", "�ɶ�", "����", "����",
								 "��ɳ", "�Ͼ�", "�人", "���", "����", "����", "����", "֣��" };
	// ��ӳ���
	for (int i = 0; i < 15; i++) {
		addCity(map, cityNames[i]);
	}

	// ���·�ߣ�ʹ��������ɵ�ʱ��ͳɱ�
	srand(time(0));  // �õ�ǰʱ����Ϊ���������
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 3; j++) {
			int endCityIndex = rand() % 15;  // ���ѡ��Ŀ�����
			int trans=rand() % 2;  // ���ѡ��ͨ��ʽ
			if (endCityIndex == i) {
				continue;  // ���Ŀ����о��������У�����
			}
			if(trans==0)
			{
				// ��ӻ�·��
				Transport train;
				train.transform = 0;
				train.SpendTime = rand() % 24 + 6;  // �������12-36Сʱ������ʱ��
				train.cost = rand() % 500 + 200;  // �������200-700Ԫ�����гɱ�
				addRoute(map, cityNames[i], cityNames[endCityIndex], train);
			}
			if(trans==1)
			{
				// ��ӷɻ�·��
				Transport plane;
				plane.transform = 1;
				plane.SpendTime = rand() % 5 + 1;  // �������2-7Сʱ������ʱ��
				plane.cost = rand() % 1500 + 500;  // �������500-2000Ԫ�����гɱ�
				addRoute(map, cityNames[i], cityNames[endCityIndex], plane);
			}
		}
	}
}



int main()
{
	Graph* map = initGraph();
	initMap(map);
	dispalycity(map);
	PrintAllRoutes(map);
	int choice;
	do {
		printf("\n**********��ͨ��ѯϵͳ**********\n");
		printf("1.��ӳ���\n");
		printf("2.���·��\n");
		printf("3.ɾ������\n");
		printf("4.ɾ��·��\n");
		printf("5.����Ʊ\n");
		printf("6.���ļ�����·��\n");
		printf("7.���ļ�����·��\n");
		printf("8.��ʾ����·��\n");
		printf("9.�˳�\n");
		printf("*********************************\n");
		printf("���������ѡ��");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			{
				printf("������������ƣ�\n");
				char add_cityname[50];
				fflush(stdin);
				cin >> add_cityname;
				addCity(map,add_cityname);
				break;
			}
		case 2:
			{
				printf("���������������ƣ�\n");
				char startcity[50];
				fflush(stdin);
				cin >> startcity;
				printf("�������յ�������ƣ�\n");
				char endcity[50];
				fflush(stdin);
				cin >> endcity;
				printf("�����뽻ͨ��ʽ����0Ϊ�𳵣�1Ϊ�ɻ���\n");
				Transport transtortform;
				scanf("%d", &transtortform.transform);
				printf("�����뻨�ѵ�ʱ��\n");
				scanf("%d", &transtortform.SpendTime);
				printf("�����뻨�ѵ�Ǯ\n");
				scanf("%d", &transtortform.cost);
				addRoute(map, startcity, endcity,transtortform);
				break;
			}
		case 3:
			{
				printf("������������ƣ�\n");
				char del_cityname[50];
				fflush(stdin);
				cin >> del_cityname;
				deleteCity(map, del_cityname);
			}
			break;
		case 4:
			{
				printf("������ɾ�������������ƣ�\n");
				char del_startcity[50];
				fflush(stdin);
				cin >> del_startcity;
				printf("������ɾ�����յ�������ƣ�\n");
				char del_endcity[50];
				fflush(stdin);
				cin >> del_endcity;
				deleteRoute(map, del_startcity, del_endcity);
				break;
			}
		case 5:
			{
				printf("���������������ƣ�\n");
				char buy_startcity[50];
				fflush(stdin);
				cin >> buy_startcity;
				printf("�������յ�������ƣ�\n");
				char buy_endcity[50];
				fflush(stdin);
				cin >> buy_endcity;
				printf("�����뽻ͨ��ʽ����0Ϊ�𳵣�1Ϊ�ɻ���\n");
				int buy_transform;
				scanf("%d", &buy_transform);
				printf("��ѡ�����·�ߣ������·�ߣ�������ת·�ߣ���1Ϊ��죬2Ϊ����ˣ�3Ϊ������ת��\n");
				int trans_choice;
				scanf("%d", &trans_choice);
				if(trans_choice==1)
				{
					fastestRoute(map, buy_startcity, buy_endcity, buy_transform);
				}
				else if(trans_choice==2)
				{
					cheapestRoute(map, buy_startcity, buy_endcity, buy_transform);
				}
				else if(trans_choice==3)
				{
					leastTransfersRoute(map, buy_startcity, buy_endcity, buy_transform);
				}
				else
				{
					printf("����������������룡\n");
				}
				break;
			}
		case 6:
			{
				printf("�������ļ�����\n");
				char filename[50];
				fflush(stdin);
				cin >> filename;
				loadGraph(map, filename);
				break;
			}
		case 7:
			{
				printf("�������ļ�����\n");
				char filename[50];
				fflush(stdin);
				cin >> filename;
				saveGraph(map, filename);
				break;
			}
		case 8:
			{
				PrintAllRoutes(map);
				break;
			}
		case 9:
			{
			choice = 9;
			break;
			}
		default:
			{
				printf("����������������룡\n");
			}
		}
	} while (choice != 9);
	return 0;
}