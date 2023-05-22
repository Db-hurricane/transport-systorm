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
    int transform;//0��ʾ�𳵣�1��ʾ�ɻ�
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

//��ʼ��ͼ��ʮ������
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

//��ʼ����ͨͼ
void InitOListDG(map* G)
{
    int i, j, k;
    char name[20];
    printf("���������������");
    scanf_s("%d", &G->citynum);
    printf("������������ƣ�");
    for (i = 0; i < G->citynum; i++)
    {
        scanf_s("%s", name);
        strcpy_s(G->cities[i].name, name);
    }
    printf("������·��������");
    scanf_s("%d", &G->routnum);
    printf("������·����Ϣ��");
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

//��ӳ���
void AddCity(map* G)
{
	int i, j;
	char name[20];
	printf("������������ƣ�");
	scanf_s("%s", name);
	strcpy_s(G->cities[G->citynum].name, name);
	G->citynum++;
	printf("������·��������");
	scanf_s("%d", &j);
	printf("������·����Ϣ��");
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

//ɾ������
void DeleteCity(map* G)
{
	int i, j;
	char name[20];
	printf("������������ƣ�");
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


//���·��
void AddRoute(map* G)
{
	int i, j;
	printf("������·��������");
	scanf_s("%d", &j);
	printf("������·����Ϣ��");
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


//ɾ��·��
void DeleteRoute(map* G)
{
	int i, j;
	char name[20];
	printf("������·����Ϣ��");
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

//���ļ��ж�ȡ���к�·����Ϣ


//�����к�·����Ϣд���ļ�


//��ѯ���·��


//��ѯ��ʡǮ·��


//��ѯ������ת����·��


//�����/�ɻ�Ʊ


//��ʾ�˵�����ȡ�û�����

