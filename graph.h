#pragma once

#define MAXINT 96000
#define MAXINDEX 600

const int RADIUS = 30; // 气味半径 20-30较好
const int M = 1; // 蚂蚁数量
const int N = 600;

typedef struct {
	int LinkID; // 路径id
	int SourceID; // 起点id
	int DestinationID; // 目标点id
	int Cost; // 费用
	float trial; // 信息素
	float smell[600]; // 气味，smell[i] 代表 节点i 散发的气味在此边上的强度
	bool enable; // 是否有效
	int num;
	bool selected;
	float smellQ;
	int circle[200];
	int numOfCircle;
	bool hasCircle[200];
} Edge;

typedef struct{
	int CircleID;
	int edgeId[4800];
	int numOfEdge;
	int path[600];
	int numOfV;
	bool inCircle[600];
	int demandIndex[50];
	int numOfDemand;
	int demandCost[50];
	int cost;
} Circle;
/*
typedef struct {
int id;
int smellTag;
int path[RADIUS+1]; // 路径点
double smell[20]; // 每段路径的气味
int pathLength;
//	short vertexIndex[600]; // 顶点所在路径位置
} SmellPath; // 气味传递路径
*/
typedef struct {

	short id;

	Edge *edges[8]; // 出度边
	int degree; // 出度
	int retainDegree;

	Edge *in_edges[600]; // 入度边
	int in_degree; // 入度
	//	float trial; // 信息素
	//	double smell[600];
	//	int smellPath[100000]; // smellPath[i]:该顶点在气味路径i上的索引位置
} Vertex;

typedef struct graph{
	Edge *edges[4800]; // 存储边
	int numOfE;
	Vertex *vertexs[600]; // 存储节点
	int numOfV;
	Edge *vTov[600][600]; // 点到点之间的边
	int vTovCost[600][600];
	Circle *circle[5000];
	int numOfCircle;

	int SourceID; // 起点
	int DestinationId; // 终点
	bool IncludingSet[600]; // 是否是必经节点
	int demand[50]; // 存储必经节点
	int numOfDemand; // 必经节点数量
	int vDemandId[600];

	int path[600];
	int pathLength;
	bool IncludingPathSet[600];
	int currentCost;
	int crossNum;

	int minCost; // 当前最小代价
	int minPath[600]; // 最小代价路径
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
	int index_id[4800];
	int id_index[4800];
} Graph;

// 读图
Graph* readGraph(const char *graphFile, const char *pathFile); 

// 获取时间
long getTime(); 

// 检验路径有效性
bool check(int path[600], int pathLength, Graph *graph); 

bool checkCircle(Graph *graph, const double *result, int length);
void preDelCircle(short k, short src, Graph *graph);
