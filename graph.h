#pragma once

#include "CONST.h"
#include "path.h"

const int RADIUS = 30; // 气味半径 20-30较好
const int M = 1; // 蚂蚁数量
const int N = 600;

typedef struct {
	ID LinkID; // 路径id
	ID SourceID; // 起点id
	ID DestinationID; // 目标点id
	int Cost; // 费用

	float trial; // 信息素
	float smell[MAX_INDEX]; // 气味，smell[i] 代表 节点i 散发的气味在此边上的强度
	bool enable; // 是否有效
	int num;
	bool selected;
	float smellQ;
} Edge;

typedef struct{
	int CircleID;
	int edgeId[MAX_EDGE];
	int numOfEdge;
	int path[MAX_INDEX];
	int numOfV;
	bool inCircle[MAX_INDEX];
	int demandIndex[MAX_DEMAND];
	int numOfDemand;
	int demandCost[MAX_DEMAND];
	int cost;
} Circle;

typedef struct {

	ID id;

	Edge *edges[MAX_DEGREE]; // 出度边
	char degree; // 出度
	char retainDegree;

	Edge *in_edges[MAX_INDEX]; // 入度边
	ID in_degree; // 入度
	//	float trial; // 信息素
	//	double smell[600];
	//	int smellPath[100000]; // smellPath[i]:该顶点在气味路径i上的索引位置
} Vertex;

typedef struct graph{
	Edge *edges[MAX_EDGE]; // 存储边
	ID numOfE;
	Vertex *vertexs[MAX_INDEX]; // 存储节点
	ID numOfV;
	Edge *vTov[MAX_INDEX][MAX_INDEX]; // 点到点之间的边
	Path_* tsp[MAX_INDEX][MAX_INDEX];
	int vTovCost[MAX_INDEX][MAX_INDEX];
	Circle *circle[5000];
	int numOfCircle;

	ID SourceID; // 起点
	ID DestinationId; // 终点
	bool *IncludingSet; // 必经点集指针
	ID numOfDemand; // 必经点数量指针
	ID *demand; // 必经点数组指针

	bool Including[2][MAX_INDEX]; // 是否是必经节点
	ID demands[2][MAX_DEMAND]; // 存储必经节点
	int numOfDemands[2]; // 必经节点数量
	ID vDemandId[2][MAX_DEMAND];

	ID path[MAX_INDEX];
	int pathLength;
	bool IncludingPathSet[MAX_INDEX];
	int currentCost;
	int crossNum;

	int minCost; // 当前最小代价
	ID minPath[MAX_INDEX]; // 最小代价路径
	int minpathLength; // 最小代价路径段数

	//int vcrossNum[600]; // 节点经过次数

	//	SmellPath *smellPath[1000000];
	int numOfSmellPath;

	// 信息素启发因子
	double ALPHA;
	// 期望启发因子
	double BETA;
	// 残留系数
	// 1-ROU 为挥发系数
	double ROU;

	long startTime; // 生成时间
	int resultNum;
	int selectedNum;

	// work with solver
	ID index_id[MAX_EDGE];
	ID id_index[MAX_EDGE];
} Graph;

// 读图
Graph* readGraph(const char *graphFile, const char *pathFile); 

// 获取时间
long getTime(); 

// 检验路径有效性
bool check(ID *path, int pathLength, Graph *graph); 

bool checkCircle(Graph *graph, const double *result, int length);
void preDelCircle(short k, short src, Graph *graph);

void setDemandId(Graph *graph,ID i);
void buildKSP(short k, short src, short dst, Graph *graph);
void genSmell(Graph *graph, Vertex *vertex, int smellTag, int distance, int *path, int length, short *vIndex);
