#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#define Max 100
#define INT_MAX 2147483647

typedef struct Transport
{
    int cost;
    int SpendTime;
    int transform; // 0��ʾ�𳵣�1��ʾ�ɻ�
} Transport;

typedef struct Route
{
    int tail, head;
    Transport transport;
    struct Route* tlink, * hlink;
} Route;

typedef struct City
{
    char name[20];
    Route* firstin, * firstout;
} City;

typedef struct Graph
{
    City cities[Max];
    int citynum, routenum;
} Map;

// ��ʼ��ͼ��ʮ������
Map* initGraph() {
    Map* map = (Map*)malloc(sizeof(Map));
    if (map == NULL) {
        return NULL;
    }
    map->citynum = 0;
    map->routenum = 0;
    return map;
}

// ��ӳ���
int addCity(Map* map, const char* cityName) {
    if (map->citynum >= Max) {
        return -1; // �ﵽ������������
    }
    // ����Ƿ��Ѿ�����ͬ������
    strncpy(map->cities[map->citynum].name, cityName, 20);
    map->cities[map->citynum].firstin = NULL;
    map->cities[map->citynum].firstout = NULL;
    map->citynum++;
    return 0; // ��ӳɹ�
}

// ɾ�����У�������ɾ����
int deleteCity(Map* map, const char* cityName) {
    for (int i = 0; i < map->citynum; i++) {
        if (strncmp(map->cities[i].name, cityName, 20) == 0) {
            // �ҵ����У�ɾ������ͨ����Ҫ����Ĳ��ֱȽϸ��ӣ���Ҫ���ǳ��м��·�ߵ�����
            // ����ֻ�򵥵�ͨ�������һ�������Ƶ���ɾ����λ��������ɾ��
            map->cities[i] = map->cities[map->citynum - 1];
            map->citynum--;
            return 0; // ɾ���ɹ�
        }
    }
    return -1; // δ�ҵ�����
}

// ���·�ߣ�����ֻ��ӵ���·�ߣ�˫��·����Ҫ�������Σ�
int addRoute(Map* map, const char* startCityName, const char* endCityName, Transport transport) {
    Route* route = (Route*)malloc(sizeof(Route));
    if (route == NULL) {
        return -1; // �ڴ����ʧ��
    }
    // �ҵ���ʼ���к���ֹ���е�����
    int startCityIndex = -1, endCityIndex = -1;
    for (int i = 0; i < map->citynum; i++) {
        // �������������Ʋ����ظ�
        if (strncmp(map->cities[i].name, startCityName, 20) == 0) {
            startCityIndex = i;
        }
        else if (strncmp(map->cities[i].name, endCityName, 20) == 0) {
            endCityIndex = i;
        }
    }
    // ���δ�ҵ����У���Ҫ�ͷ��ڴ�
    if (startCityIndex == -1 || endCityIndex == -1) {
        free(route);
        return -1;
    }
    // �������·�߲����ڣ����·���Ѵ��ڣ���Ҫ��ɾ��ԭ��·��
    route->head = endCityIndex;
    route->tail = startCityIndex;
    route->transport = transport;
    route->tlink = map->cities[startCityIndex].firstout;
    route->hlink = map->cities[endCityIndex].firstin;
    map->cities[startCityIndex].firstout = route;
    map->cities[endCityIndex].firstin = route;
    map->routenum++;
    return 0; // ��ӳɹ�
}

// ɾ��·�ߣ�����ʼ���к���ֹ����ɾ����
int deleteRoute(Map* map, const char* startCityName, const char* endCityName) {
    // �ҵ���ʼ���к���ֹ���е�����
    int startCityIndex = -1, endCityIndex = -1;
    for (int i = 0; i < map->citynum; i++) {
        if (strncmp(map->cities[i].name, startCityName, 20) == 0) {
            startCityIndex = i;
        }
        else if (strncmp(map->cities[i].name, endCityName, 20) == 0) {
            endCityIndex = i;
        }
    }
    if (startCityIndex == -1 || endCityIndex == -1) {
        return -1; // δ�ҵ�����
    }
    // �ҵ�·�߲�ɾ��
    Route* current = map->cities[startCityIndex].firstout;
    Route* prev = NULL;
    // �������·�߲�����
    while (current != NULL && current->head != endCityIndex) {
        prev = current;
        current = current->tlink;
    }
    if (current == NULL) {
        return -1; // δ�ҵ�·��
    }
    // ɾ��·��
    if (prev == NULL) {
        map->cities[startCityIndex].firstout = current->tlink;
    }
    else {
        prev->tlink = current->tlink;
    }
    prev = NULL;
    // �������·�߲�����
    Route* temp = map->cities[endCityIndex].firstin;
    while (temp != current) {
        prev = temp;
        temp = temp->hlink;
    }
    // ɾ��·��
    if (prev == NULL) {
        map->cities[endCityIndex].firstin = current->hlink;
    }
    else {
        prev->hlink = current->hlink;
    }
    free(current);
    map->routenum--;
    return 0; // ɾ���ɹ�
}


// ���ļ��ж�ȡ���к�·����Ϣ
int loadGraph(Map* map, const char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // ���ļ�ʧ��
    }
    char line[100];
    while (fgets(line, 100, file) != NULL) {
        // �������ÿ�еĸ�ʽ���£�
        // ���ڳ��У�������
        // ����·�ߣ���ʼ����,��ֹ����,��ͨ����,����,����ʱ��
        if (strchr(line, ',') == NULL) {
            // ����һ������
            line[strcspn(line, "\n")] = 0; // ȥ�����з�
            addCity(map, line);
        }
        else {
            // ����һ��·��
            char startCity[20], endCity[20];
            Transport transport;
            // �������·����Ϣ��ʽ��ȷ
            sscanf(line, "%[^,],%[^,],%d,%d,%d\n", startCity, endCity, &transport.transform, &transport.cost, &transport.SpendTime);
            addRoute(map, startCity, endCity, transport);
        }
    }
    fclose(file);
    return 0;
}

// �����к�·����Ϣд���ļ�
int saveGraph(Map* map, const char* filename) {
    FILE* file;
    file = fopen(filename, "w");
    if (file == NULL) {
        return -1; // ���ļ�ʧ��
    }
    for (int i = 0; i < map->citynum; i++) {
        // д�������Ϣ
        fprintf(file, "%s\n", map->cities[i].name);
    }
    for (int i = 0; i < map->citynum; i++) {
        // д��·����Ϣ
        Route* route = map->cities[i].firstout;
        while (route != NULL) {
            // �������·����Ϣ��ʽ��ȷ
            fprintf(file, "%s,%s,%d,%d,%d\n", map->cities[route->tail].name, map->cities[route->head].name, route->transport.transform, route->transport.cost, route->transport.SpendTime);
            route = route->tlink;
        }
    }
    fclose(file);
    return 0;
}

//����Ȩ��ѡ����С��·��
void Dijkstra(Map* map, int start, int end, int* dist, int* prev, const int buy_transform, int& total_time, int& total_cost) {
    int visited[Max]; // ��¼�����Ƿ񱻷��ʹ�
    // ��ʼ������
    for (int i = 0; i < map->citynum; ++i) {
        visited[i] = 0; // ��ʼ״̬�£����г��ж�δ�����ʹ�
        dist[i] = INT_MAX; // ���г��е���ʼ���еľ����ʼ��Ϊ�����
    }

    // ���ڿ�ʼ���У�����Ϊ0
    dist[start] = 0;
    total_time = 0;
    total_cost = 0;
    for (int i = 0; i < map->citynum; ++i) {
        int u = -1; // Ѱ�ҵ�ǰδ���ʳ����о�����С�ĳ���
        int min = INT_MAX;
        // �ҵ�������С�ĳ���
        for (int j = 0; j < map->citynum; ++j) {
            if (!visited[j] && dist[j] < min) {
                u = j;
                min = dist[j];
            }
        }

        if (u == -1) // ���г����ѷ�����
            return;

        visited[u] = 1; // ��ǳ����ѱ�����
        Route* route = map->cities[u].firstout; // ��ȡ�ó������г���
        while (route) {
            if (route->transport.transform == buy_transform)
            {
                int v = route->head; // ��ȡ�ߵ���һ�˳���
                if (!visited[v] && route->transport.SpendTime != INT_MAX && dist[u] + route->transport.SpendTime < dist[v])
                {
                    // ����µ�·����ԭ�е�·���̣�����·����Ϣ
                    dist[v] = dist[u] + route->transport.SpendTime;
                    prev[v] = u;
                }
            }

            route = route->tlink; // �ƶ�����һ����
        }

    }
    // �ҵ����·���󣬸�����ʱ����ܻ���
    total_time = dist[end];
    int cur = end;
    while (cur != start) {
        Route* route = map->cities[prev[cur]].firstout;
        while (route) {
            if (route->head == cur) {
                total_cost += route->transport.cost;
                break;
            }
            route = route->tlink;
        }
        cur = prev[cur];
    }
}

//��ȡͼ�г��е�λ��
int getCityIndex(Map* map, const char* cityName) {
    for (int i = 0; i < map->citynum; i++) {
        if (strcmp(map->cities[i].name, cityName) == 0) {
            return i;
        }
    }
    return -1;  //����-1��ʾû���ҵ��ó���
}

//����ѡ��Ľ�ͨ��ʽ������·��
Route* fastestRoute(Map* map, const char* startCityName, const char* endCityName,int transform)
{
    int start = getCityIndex(map, startCityName);
    int end = getCityIndex(map, endCityName);
    int dist[Max], prev[Max];
    int total_time, total_cost;
    
    Dijkstra(map, start, end, dist, prev, transform, total_time, total_cost);
    // ����յ���еľ���Ϊ�����˵��û�д���㵽�յ��·��
    if (dist[end] == INT_MAX) {
        printf("û�д�%s��%s��·��.\n", startCityName, endCityName);
        return NULL;
    }

    printf("��%s��%s�����·�߻���%dСʱ%dԪǮ.\n", startCityName, endCityName, total_time,total_cost);

    // ��ӡ·��
    int path[Max], cnt = 0;
    for (int i = end; i != start; i = prev[i])
        path[cnt++] = i;
    path[cnt] = start;

    for (int i = cnt; i > 0; --i) {
        printf("%s -> ", map->cities[path[i]].name);
    }
    printf("%s\n", map->cities[path[0]].name);

    return NULL;
}

//����ѡ��Ľ�ͨ��ʽ������˵�·��
Route* cheapestRoute(Map* map, const char* startCityName, const char* endCityName,int transform) {
    int start = getCityIndex(map, startCityName);
    int end = getCityIndex(map, endCityName);
    int dist[Max], prev[Max];
    int total_time,total_cost;

    Dijkstra(map, start, end, dist, prev, transform, total_time, total_cost);
    // ����յ���еľ���Ϊ�����˵��û�д���㵽�յ��·��
    if (dist[end] == INT_MAX) {
        printf("û�д�%s��%s��·��.\n", startCityName, endCityName);
        return NULL;
    }

    printf("��%s��%s�������·�߻���%dСʱ%dԪǮ.\n", startCityName, endCityName, total_time, total_cost);

    // ��ӡ·��
    int path[Max], cnt = 0;
    for (int i = end; i != start; i = prev[i])
        path[cnt++] = i;
    path[cnt] = start;

    for (int i = cnt; i > 0; --i) {
        printf("%s -> ", map->cities[path[i]].name);
    }
    printf("%s\n", map->cities[path[0]].name);

    return NULL;
}


//����ѡ��Ľ�ͨ��ʽ��ѡ��������ת��·��
Route* leastTransfersRoute(Map* map, const char* startCityName, const char* endCityName,int transform) {
    int start = getCityIndex(map, startCityName);
    int end = getCityIndex(map, endCityName);
    int dist[Max], prev[Max];
    int total_time = 0, total_cost = 0;
    for (int i = 0; i < map->citynum; i++) {
        dist[i] = INT_MAX;  // ��ʼ�����г��е���ת����Ϊ�����
    }
    dist[start] = 0;  // �����е���ת����Ϊ0

    // ʹ�ö���ʵ��BFS
    int queue[Max];
    int head = 0, tail = 0;
    queue[tail++] = start;  // ���������

    while (head < tail) {
        int u = queue[head++];  // ����
        Route* route = map->cities[u].firstout;
        // �������д�u�����ı�
        while (route) {
            // ����ߵ���һ�˳��е���ת��������u����ת������1��������ת����
            if(route->transport.transform==transform)
            {
                int v = route->head;
                if (dist[u] + 1 < dist[v]) {  // ����µ���ת������ԭ�е���ת�����٣�������ת����
                    dist[v] = dist[u] + 1;
                    prev[v] = u;
                    total_time += route->transport.SpendTime;
                    total_cost += route->transport.cost;
                    queue[tail++] = v;  // �µĳ������
                }
            }
            // ��һ����
            route = route->tlink;
        }
    }
    // ����յ���е���ת����Ϊ�����˵��û�д���㵽�յ��·��
    if (dist[end] == INT_MAX) {
        printf("No available route from %s to %s.\n", startCityName, endCityName);
        return NULL;
    }

    printf("��%s��%s��������ת·����ת%d��.�ܹ�������%dСʱʱ�䣬%dԪǮ\n", startCityName, endCityName, dist[end],total_time,total_cost);

    // ��ӡ·��
    int path[Max], cnt = 0;
    for (int i = end; i != start; i = prev[i])
        path[cnt++] = i;
    path[cnt] = start;
    // �����ӡ·��
    for (int i = cnt; i > 0; --i) {
        printf("%s -> ", map->cities[path[i]].name);
    }
    printf("%s\n", map->cities[path[0]].name);

    return NULL;
}

//չʾ���еĳ���
void dispalycity(Map* map)
{
    int count = 0;
	for (int i = 0; i < map->citynum; i++)
	{
		printf("%s\t", map->cities[i].name);
        count++;
        if(count%5==0)
        {
	        printf("\n");
        }
	}
}

// ��ӡ���л𳵺ͷɻ���·��
void PrintAllRoutes(Map* map) {
    // �������еĳ���
    for (int i = 0; i < map->citynum; ++i) {
        Route* route = map->cities[i].firstout; // ��ȡ�ó��е����г���
        // �������е�·��
        while (route) {
            // ����ǻ𳵻�ɻ���·�ߣ��ʹ�ӡ����
            if (route->transport.transform == 0) {
                printf("��·��: %s --> %s, ����ʱ��: %d, ���ѷ���: %d\n",
                    map->cities[i].name, map->cities[route->head].name, route->transport.SpendTime, route->transport.cost);
            }
            else if (route->transport.transform == 1) {
                printf("�ɻ�·��: %s --> %s, ����ʱ��: %d, ���ѷ���: %d\n",
                    map->cities[i].name, map->cities[route->head].name, route->transport.SpendTime, route->transport.cost);
            }
            route = route->tlink; // �ƶ�����һ����
        }
    }
}

