#pragma once

#include "CONST.h"
#include "path.h"

const int RADIUS = 30; // ��ζ�뾶 20-30�Ϻ�
const int M = 1; // ��������
const int N = 600;

typedef struct {
	ID LinkID; // ·��id
	ID SourceID; // ���id
	ID DestinationID; // Ŀ���id
	int Cost; // ����

	float trial; // ��Ϣ��
	float smell[MAX_INDEX]; // ��ζ��smell[i] ���� �ڵ�i ɢ������ζ�ڴ˱��ϵ�ǿ��
	bool enable; // �Ƿ���Ч
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

	Edge *edges[MAX_DEGREE]; // ���ȱ�
	char degree; // ����
	char retainDegree;

	Edge *in_edges[MAX_INDEX]; // ��ȱ�
	ID in_degree; // ���
	//	float trial; // ��Ϣ��
	//	double smell[600];
	//	int smellPath[100000]; // smellPath[i]:�ö�������ζ·��i�ϵ�����λ��
} Vertex;

typedef struct graph{
	Edge *edges[MAX_EDGE]; // �洢��
	ID numOfE;
	Vertex *vertexs[MAX_INDEX]; // �洢�ڵ�
	ID numOfV;
	Edge *vTov[MAX_INDEX][MAX_INDEX]; // �㵽��֮��ı�
	Path_* tsp[MAX_INDEX][MAX_INDEX];
	int vTovCost[MAX_INDEX][MAX_INDEX];
	Circle *circle[5000];
	int numOfCircle;

	ID SourceID; // ���
	ID DestinationId; // �յ�
	bool *IncludingSet; // �ؾ��㼯ָ��
	ID numOfDemand; // �ؾ�������ָ��
	ID *demand; // �ؾ�������ָ��

	bool Including[2][MAX_INDEX]; // �Ƿ��Ǳؾ��ڵ�
	ID demands[2][MAX_DEMAND]; // �洢�ؾ��ڵ�
	int numOfDemands[2]; // �ؾ��ڵ�����
	ID vDemandId[2][MAX_DEMAND];

	ID path[MAX_INDEX];
	int pathLength;
	bool IncludingPathSet[MAX_INDEX];
	int currentCost;
	int crossNum;

	int minCost; // ��ǰ��С����
	ID minPath[MAX_INDEX]; // ��С����·��
	int minpathLength; // ��С����·������

	//int vcrossNum[600]; // �ڵ㾭������

	//	SmellPath *smellPath[1000000];
	int numOfSmellPath;

	// ��Ϣ����������
	double ALPHA;
	// ������������
	double BETA;
	// ����ϵ��
	// 1-ROU Ϊ�ӷ�ϵ��
	double ROU;

	long startTime; // ����ʱ��
	int resultNum;
	int selectedNum;

	// work with solver
	ID index_id[MAX_EDGE];
	ID id_index[MAX_EDGE];
} Graph;

// ��ͼ
Graph* readGraph(const char *graphFile, const char *pathFile); 

// ��ȡʱ��
long getTime(); 

// ����·����Ч��
bool check(ID *path, int pathLength, Graph *graph); 

bool checkCircle(Graph *graph, const double *result, int length);
void preDelCircle(short k, short src, Graph *graph);

void setDemandId(Graph *graph,ID i);
void buildKSP(short k, short src, short dst, Graph *graph);
void genSmell(Graph *graph, Vertex *vertex, int smellTag, int distance, int *path, int length, short *vIndex);
