#include"graph.h"
#include <time.h>

void initMap(Map* map) {
	// 城市名字列表
	const char* cityNames[15] = { "北京", "上海", "广州", "深圳", "成都", "杭州", "西安",
								 "长沙", "南京", "武汉", "天津", "重庆", "济南", "长春", "郑州" };
	// 添加城市
	for (int i = 0; i < 15; i++) {
		addCity(map, cityNames[i]);
	}

	// 添加路线，使用随机生成的时间和成本
	srand(time(0));  // 用当前时间作为随机数种子
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 3; j++) {
			int endCityIndex = rand() % 15;  // 随机选择目标城市
			int trans=rand() % 2;  // 随机选择交通方式
			if (endCityIndex == i) {
				continue;  // 如果目标城市就是起点城市，跳过
			}
			if(trans==0)
			{
				// 添加火车路线
				Transport train;
				train.transform = 0;
				train.SpendTime = rand() % 24 + 6;  // 随机生成12-36小时的旅行时间
				train.cost = rand() % 500 + 200;  // 随机生成200-700元的旅行成本
				addRoute(map, cityNames[i], cityNames[endCityIndex], train);
			}
			if(trans==1)
			{
				// 添加飞机路线
				Transport plane;
				plane.transform = 1;
				plane.SpendTime = rand() % 5 + 1;  // 随机生成2-7小时的旅行时间
				plane.cost = rand() % 1500 + 500;  // 随机生成500-2000元的旅行成本
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
		printf("\n**********交通咨询系统**********\n");
		printf("1.添加城市\n");
		printf("2.添加路线\n");
		printf("3.删除城市\n");
		printf("4.删除路线\n");
		printf("5.购买车票\n");
		printf("6.用文件导入路线\n");
		printf("7.用文件导出路线\n");
		printf("8.显示所有路线\n");
		printf("9.退出\n");
		printf("*********************************\n");
		printf("请输入你的选择：");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			{
				printf("请输入城市名称：\n");
				char add_cityname[50];
				fflush(stdin);
				cin >> add_cityname;
				addCity(map,add_cityname);
				break;
			}
		case 2:
			{
				printf("请输入起点城市名称：\n");
				char startcity[50];
				fflush(stdin);
				cin >> startcity;
				printf("请输入终点城市名称：\n");
				char endcity[50];
				fflush(stdin);
				cin >> endcity;
				printf("请输入交通方式：（0为火车，1为飞机）\n");
				Transport transtortform;
				scanf("%d", &transtortform.transform);
				printf("请输入花费的时间\n");
				scanf("%d", &transtortform.SpendTime);
				printf("请输入花费的钱\n");
				scanf("%d", &transtortform.cost);
				addRoute(map, startcity, endcity,transtortform);
				break;
			}
		case 3:
			{
				printf("请输入城市名称：\n");
				char del_cityname[50];
				fflush(stdin);
				cin >> del_cityname;
				deleteCity(map, del_cityname);
			}
			break;
		case 4:
			{
				printf("请输入删除的起点城市名称：\n");
				char del_startcity[50];
				fflush(stdin);
				cin >> del_startcity;
				printf("请输入删除的终点城市名称：\n");
				char del_endcity[50];
				fflush(stdin);
				cin >> del_endcity;
				deleteRoute(map, del_startcity, del_endcity);
				break;
			}
		case 5:
			{
				printf("请输入起点城市名称：\n");
				char buy_startcity[50];
				fflush(stdin);
				cin >> buy_startcity;
				printf("请输入终点城市名称：\n");
				char buy_endcity[50];
				fflush(stdin);
				cin >> buy_endcity;
				printf("请输入交通方式：（0为火车，1为飞机）\n");
				int buy_transform;
				scanf("%d", &buy_transform);
				printf("请选择最快路线，最便宜路线，最少中转路线：（1为最快，2为最便宜，3为最少中转）\n");
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
					printf("输入错误，请重新输入！\n");
				}
				break;
			}
		case 6:
			{
				printf("请输入文件名：\n");
				char filename[50];
				fflush(stdin);
				cin >> filename;
				loadGraph(map, filename);
				break;
			}
		case 7:
			{
				printf("请输入文件名：\n");
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
				printf("输入错误，请重新输入！\n");
			}
		}
	} while (choice != 9);
	return 0;
}