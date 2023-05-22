#include<stdio.h>
#include<string.h>
#include<malloc.h>

#define Max 50


typedef struct Transport
{
    char StartCity[20];
    char EndCity[20];
    int cost;
    int SpendTime;
    int transform;//0表示火车，1表示飞机
};

typedef struct Route
{
    int tail, head;

    struct Route* tlink, * hlink;
}route;

typedef struct City
{
    char name[20];
    //struct Transport* transports=malloc(numbers_of*sizeof(struct Transport));
    Route* firstin, * firstout;
}city;

typedef struct Graph
{
    City cities[Max];
    int citynum, routnum;
}map;

//初始化图的十字链表
void InitMap(Graph* map)
{
    map->citynum = 0;
    map->routnum = 0;
    for (int i = 0; i < Max; i++)
    {
        map->cities[i].firstin = NULL;
        map->cities[i].firstout = NULL;
    };
}

//初始化交通图
void InitOListDG(map* G)
{
    int i, j, k;
    char name[20];
    printf("请输入城市数量：");
    scanf_s("%d", &G->citynum);
    printf("请输入城市名称：");
    for (i = 0; i < G->citynum; i++)
    {
        scanf_s("%s", name);
        strcpy_s(G->cities[i].name, name);
    }
    printf("请输入路线数量：");
    scanf_s("%d", &G->routnum);
    printf("请输入路线信息：");
    for (k = 0; k < G->routnum; k++)
    {
        Transport* transport = (Transport*)malloc(sizeof(Transport));
        scanf_s("%s %s %d %d %d", transport->StartCity, transport->EndCity, &transport->cost, &transport->SpendTime, &transport->transform);
        for (i = 0; i < G->citynum; i++)
        {
            if (strcmp(G->cities[i].name, transport->StartCity) == 0)
            {
                for (j = 0; j < G->citynum; j++)
                {
                    if (strcmp(G->cities[j].name, transport->EndCity) == 0)
                    {
                        Route* p = (Route*)malloc(sizeof(Route));
                        p->head = j;
                        p->tail = i;
                        p->tlink = G->cities[i].firstout;
                        p->hlink = G->cities[j].firstin;
                        G->cities[i].firstout = p;
                        G->cities[j].firstin = p;
                    }
                }
            }
        }
    }
}

//添加城市
void AddCity(map* G)
{
	int i, j;
	char name[20];
	printf("请输入城市名称：");
	scanf_s("%s", name);
	strcpy_s(G->cities[G->citynum].name, name);
	G->citynum++;
	printf("请输入路线数量：");
	scanf_s("%d", &j);
	printf("请输入路线信息：");
	for (i = 0; i < j; i++)
	{
		Transport* transport = (Transport*)malloc(sizeof(Transport));
		scanf_s("%s %s %d %d %d", transport->StartCity, transport->EndCity, &transport->cost, &transport->SpendTime, &transport->transform);
		for (i = 0; i < G->citynum; i++)
		{
			if (strcmp(G->cities[i].name, transport->StartCity) == 0)
			{
				for (j = 0; j < G->citynum; j++)
				{
					if (strcmp(G->cities[j].name, transport->EndCity) == 0)
					{
						Route* p = (Route*)malloc(sizeof(Route));
						p->head = j;
						p->tail = i;
						p->tlink = G->cities[i].firstout;
						p->hlink = G->cities[j].firstin;
						G->cities[i].firstout = p;
						G->cities[j].firstin = p;
					}
				}
			}
		}
	}
}

//删除城市
void DeleteCity(map* G)
{
	int i, j;
	char name[20];
	printf("请输入城市名称：");
	scanf_s("%s", name);
	for (i = 0; i < G->citynum; i++)
	{
		if (strcmp(G->cities[i].name, name) == 0)
		{
			for (j = 0; j < G->citynum; j++)
			{
				if (strcmp(G->cities[j].name, name) == 0)
				{
					Route* p = G->cities[i].firstout;
					while (p != NULL)
					{
						if (p->head == j)
						{
							p->tail = -1;
							p->head = -1;
						}
						p = p->tlink;
					}
					p = G->cities[j].firstin;
					while (p != NULL)
					{
						if (p->tail == i)
						{
							p->tail = -1;
							p->head = -1;
						}
						p = p->hlink;
					}
				}
			}
		}
	}
}


//添加路线
void AddRoute(map* G)
{
	int i, j;
	printf("请输入路线数量：");
	scanf_s("%d", &j);
	printf("请输入路线信息：");
	for (i = 0; i < j; i++)
	{
		Transport* transport = (Transport*)malloc(sizeof(Transport));
		scanf_s("%s %s %d %d %d", transport->StartCity, transport->EndCity, &transport->cost, &transport->SpendTime, &transport->transform);
		for (i = 0; i < G->citynum; i++)
		{
			if (strcmp(G->cities[i].name, transport->StartCity) == 0)
			{
				for (j = 0; j < G->citynum; j++)
				{
					if (strcmp(G->cities[j].name, transport->EndCity) == 0)
					{
						Route* p = (Route*)malloc(sizeof(Route));
						p->head = j;
						p->tail = i;
						p->tlink = G->cities[i].firstout;
						p->hlink = G->cities[j].firstin;
						G->cities[i].firstout = p;
						G->cities[j].firstin = p;
					}
				}
			}
		}
	}
}


//删除路线
void DeleteRoute(map* G)
{
	int i, j;
	char name[20];
	printf("请输入路线信息：");
	scanf_s("%s %s", name);
	for (i = 0; i < G->citynum; i++)
	{
		if (strcmp(G->cities[i].name, name) == 0)
		{
			for (j = 0; j < G->citynum; j++)
			{
				if (strcmp(G->cities[j].name, name) == 0)
				{
					Route* p = G->cities[i].firstout;
					while (p != NULL)
					{
						if (p->head == j)
						{
							p->tail = -1;
							p->head = -1;
						}
						p = p->tlink;
					}
					p = G->cities[j].firstin;
					while (p != NULL)
					{
						if (p->tail == i)
						{
							p->tail = -1;
							p->head = -1;
						}
						p = p->hlink;
					}
				}
			}
		}
	}
}

//从文件中读取城市和路线信息


//将城市和路线信息写入文件


//查询最快路线


//查询最省钱路线


//查询最少中转次数路线


//购买火车/飞机票


//显示菜单并获取用户输入

