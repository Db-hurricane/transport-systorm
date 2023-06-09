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
    int transform; // 0表示火车，1表示飞机
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

// 初始化图的十字链表
Map* initGraph() {
    Map* map = (Map*)malloc(sizeof(Map));
    if (map == NULL) {
        return NULL;
    }
    map->citynum = 0;
    map->routenum = 0;
    return map;
}

// 添加城市
int addCity(Map* map, const char* cityName) {
    if (map->citynum >= Max) {
        return -1; // 达到城市数量上限
    }
    // 检查是否已经存在同名城市
    strncpy(map->cities[map->citynum].name, cityName, 20);
    map->cities[map->citynum].firstin = NULL;
    map->cities[map->citynum].firstout = NULL;
    map->citynum++;
    return 0; // 添加成功
}

// 删除城市（按名称删除）
int deleteCity(Map* map, const char* cityName) {
    for (int i = 0; i < map->citynum; i++) {
        if (strncmp(map->cities[i].name, cityName, 20) == 0) {
            // 找到城市，删除操作通常需要处理的部分比较复杂，需要考虑城市间的路线等问题
            // 这里只简单地通过将最后一个城市移到被删除的位置来进行删除
            map->cities[i] = map->cities[map->citynum - 1];
            map->citynum--;
            return 0; // 删除成功
        }
    }
    return -1; // 未找到城市
}

// 添加路线（这里只添加单向路线，双向路线需要调用两次）
int addRoute(Map* map, const char* startCityName, const char* endCityName, Transport transport) {
    Route* route = (Route*)malloc(sizeof(Route));
    if (route == NULL) {
        return -1; // 内存分配失败
    }
    // 找到起始城市和终止城市的索引
    int startCityIndex = -1, endCityIndex = -1;
    for (int i = 0; i < map->citynum; i++) {
        // 这里假设城市名称不会重复
        if (strncmp(map->cities[i].name, startCityName, 20) == 0) {
            startCityIndex = i;
        }
        else if (strncmp(map->cities[i].name, endCityName, 20) == 0) {
            endCityIndex = i;
        }
    }
    // 如果未找到城市，需要释放内存
    if (startCityIndex == -1 || endCityIndex == -1) {
        free(route);
        return -1;
    }
    // 这里假设路线不存在，如果路线已存在，需要先删除原有路线
    route->head = endCityIndex;
    route->tail = startCityIndex;
    route->transport = transport;
    route->tlink = map->cities[startCityIndex].firstout;
    route->hlink = map->cities[endCityIndex].firstin;
    map->cities[startCityIndex].firstout = route;
    map->cities[endCityIndex].firstin = route;
    map->routenum++;
    return 0; // 添加成功
}

// 删除路线（按起始城市和终止城市删除）
int deleteRoute(Map* map, const char* startCityName, const char* endCityName) {
    // 找到起始城市和终止城市的索引
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
        return -1; // 未找到城市
    }
    // 找到路线并删除
    Route* current = map->cities[startCityIndex].firstout;
    Route* prev = NULL;
    // 这里假设路线不存在
    while (current != NULL && current->head != endCityIndex) {
        prev = current;
        current = current->tlink;
    }
    if (current == NULL) {
        return -1; // 未找到路线
    }
    // 删除路线
    if (prev == NULL) {
        map->cities[startCityIndex].firstout = current->tlink;
    }
    else {
        prev->tlink = current->tlink;
    }
    prev = NULL;
    // 这里假设路线不存在
    Route* temp = map->cities[endCityIndex].firstin;
    while (temp != current) {
        prev = temp;
        temp = temp->hlink;
    }
    // 删除路线
    if (prev == NULL) {
        map->cities[endCityIndex].firstin = current->hlink;
    }
    else {
        prev->hlink = current->hlink;
    }
    free(current);
    map->routenum--;
    return 0; // 删除成功
}


// 从文件中读取城市和路线信息
int loadGraph(Map* map, const char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // 打开文件失败
    }
    char line[100];
    while (fgets(line, 100, file) != NULL) {
        // 这里假设每行的格式如下：
        // 对于城市：城市名
        // 对于路线：起始城市,终止城市,交通工具,费用,所需时间
        if (strchr(line, ',') == NULL) {
            // 这是一个城市
            line[strcspn(line, "\n")] = 0; // 去掉换行符
            addCity(map, line);
        }
        else {
            // 这是一条路线
            char startCity[20], endCity[20];
            Transport transport;
            // 这里假设路线信息格式正确
            sscanf(line, "%[^,],%[^,],%d,%d,%d\n", startCity, endCity, &transport.transform, &transport.cost, &transport.SpendTime);
            addRoute(map, startCity, endCity, transport);
        }
    }
    fclose(file);
    return 0;
}

// 将城市和路线信息写入文件
int saveGraph(Map* map, const char* filename) {
    FILE* file;
    file = fopen(filename, "w");
    if (file == NULL) {
        return -1; // 打开文件失败
    }
    for (int i = 0; i < map->citynum; i++) {
        // 写入城市信息
        fprintf(file, "%s\n", map->cities[i].name);
    }
    for (int i = 0; i < map->citynum; i++) {
        // 写入路线信息
        Route* route = map->cities[i].firstout;
        while (route != NULL) {
            // 这里假设路线信息格式正确
            fprintf(file, "%s,%s,%d,%d,%d\n", map->cities[route->tail].name, map->cities[route->head].name, route->transport.transform, route->transport.cost, route->transport.SpendTime);
            route = route->tlink;
        }
    }
    fclose(file);
    return 0;
}

//根据权重选择最小的路线
void Dijkstra(Map* map, int start, int end, int* dist, int* prev, const int buy_transform, int& total_time, int& total_cost) {
    int visited[Max]; // 记录城市是否被访问过
    // 初始化数据
    for (int i = 0; i < map->citynum; ++i) {
        visited[i] = 0; // 初始状态下，所有城市都未被访问过
        dist[i] = INT_MAX; // 所有城市到开始城市的距离初始化为无穷大
    }

    // 对于开始城市，距离为0
    dist[start] = 0;
    total_time = 0;
    total_cost = 0;
    for (int i = 0; i < map->citynum; ++i) {
        int u = -1; // 寻找当前未访问城市中距离最小的城市
        int min = INT_MAX;
        // 找到距离最小的城市
        for (int j = 0; j < map->citynum; ++j) {
            if (!visited[j] && dist[j] < min) {
                u = j;
                min = dist[j];
            }
        }

        if (u == -1) // 所有城市已访问完
            return;

        visited[u] = 1; // 标记城市已被访问
        Route* route = map->cities[u].firstout; // 获取该城市所有出边
        while (route) {
            if (route->transport.transform == buy_transform)
            {
                int v = route->head; // 获取边的另一端城市
                if (!visited[v] && route->transport.SpendTime != INT_MAX && dist[u] + route->transport.SpendTime < dist[v])
                {
                    // 如果新的路径比原有的路径短，更新路径信息
                    dist[v] = dist[u] + route->transport.SpendTime;
                    prev[v] = u;
                }
            }

            route = route->tlink; // 移动到下一个边
        }

    }
    // 找到最短路径后，更新总时间和总花费
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

//获取图中城市的位置
int getCityIndex(Map* map, const char* cityName) {
    for (int i = 0; i < map->citynum; i++) {
        if (strcmp(map->cities[i].name, cityName) == 0) {
            return i;
        }
    }
    return -1;  //返回-1表示没有找到该城市
}

//根据选择的交通方式，最快的路线
Route* fastestRoute(Map* map, const char* startCityName, const char* endCityName,int transform)
{
    int start = getCityIndex(map, startCityName);
    int end = getCityIndex(map, endCityName);
    int dist[Max], prev[Max];
    int total_time, total_cost;
    
    Dijkstra(map, start, end, dist, prev, transform, total_time, total_cost);
    // 如果终点城市的距离为无穷大，说明没有从起点到终点的路线
    if (dist[end] == INT_MAX) {
        printf("没有从%s到%s的路线.\n", startCityName, endCityName);
        return NULL;
    }

    printf("从%s到%s的最快路线花费%d小时%d元钱.\n", startCityName, endCityName, total_time,total_cost);

    // 打印路径
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

//根据选择的交通方式，最便宜的路线
Route* cheapestRoute(Map* map, const char* startCityName, const char* endCityName,int transform) {
    int start = getCityIndex(map, startCityName);
    int end = getCityIndex(map, endCityName);
    int dist[Max], prev[Max];
    int total_time,total_cost;

    Dijkstra(map, start, end, dist, prev, transform, total_time, total_cost);
    // 如果终点城市的距离为无穷大，说明没有从起点到终点的路线
    if (dist[end] == INT_MAX) {
        printf("没有从%s到%s的路线.\n", startCityName, endCityName);
        return NULL;
    }

    printf("从%s到%s的最便宜路线花费%d小时%d元钱.\n", startCityName, endCityName, total_time, total_cost);

    // 打印路径
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


//根据选择的交通方式，选择最少中转的路线
Route* leastTransfersRoute(Map* map, const char* startCityName, const char* endCityName,int transform) {
    int start = getCityIndex(map, startCityName);
    int end = getCityIndex(map, endCityName);
    int dist[Max], prev[Max];
    int total_time = 0, total_cost = 0;
    for (int i = 0; i < map->citynum; i++) {
        dist[i] = INT_MAX;  // 初始化所有城市的中转次数为无穷大
    }
    dist[start] = 0;  // 起点城市的中转次数为0

    // 使用队列实现BFS
    int queue[Max];
    int head = 0, tail = 0;
    queue[tail++] = start;  // 起点城市入队

    while (head < tail) {
        int u = queue[head++];  // 出队
        Route* route = map->cities[u].firstout;
        // 遍历所有从u出发的边
        while (route) {
            // 如果边的另一端城市的中转次数大于u的中转次数加1，更新中转次数
            if(route->transport.transform==transform)
            {
                int v = route->head;
                if (dist[u] + 1 < dist[v]) {  // 如果新的中转次数比原有的中转次数少，更新中转次数
                    dist[v] = dist[u] + 1;
                    prev[v] = u;
                    total_time += route->transport.SpendTime;
                    total_cost += route->transport.cost;
                    queue[tail++] = v;  // 新的城市入队
                }
            }
            // 下一条边
            route = route->tlink;
        }
    }
    // 如果终点城市的中转次数为无穷大，说明没有从起点到终点的路线
    if (dist[end] == INT_MAX) {
        printf("No available route from %s to %s.\n", startCityName, endCityName);
        return NULL;
    }

    printf("从%s到%s的最少中转路线中转%d次.总共花费了%d小时时间，%d元钱\n", startCityName, endCityName, dist[end],total_time,total_cost);

    // 打印路径
    int path[Max], cnt = 0;
    for (int i = end; i != start; i = prev[i])
        path[cnt++] = i;
    path[cnt] = start;
    // 逆序打印路径
    for (int i = cnt; i > 0; --i) {
        printf("%s -> ", map->cities[path[i]].name);
    }
    printf("%s\n", map->cities[path[0]].name);

    return NULL;
}

//展示所有的城市
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

// 打印所有火车和飞机的路线
void PrintAllRoutes(Map* map) {
    // 遍历所有的城市
    for (int i = 0; i < map->citynum; ++i) {
        Route* route = map->cities[i].firstout; // 获取该城市的所有出边
        // 遍历所有的路线
        while (route) {
            // 如果是火车或飞机的路线，就打印出来
            if (route->transport.transform == 0) {
                printf("火车路线: %s --> %s, 花费时间: %d, 花费费用: %d\n",
                    map->cities[i].name, map->cities[route->head].name, route->transport.SpendTime, route->transport.cost);
            }
            else if (route->transport.transform == 1) {
                printf("飞机路线: %s --> %s, 花费时间: %d, 花费费用: %d\n",
                    map->cities[i].name, map->cities[route->head].name, route->transport.SpendTime, route->transport.cost);
            }
            route = route->tlink; // 移动到下一个边
        }
    }
}

