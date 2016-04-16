#pragma once

#define MAXINT 96000
#define MAXINDEX 600

const int RADIUS = 30; // ��ζ�뾶 20-30�Ϻ�
const int M = 1; // ��������
const int N = 600;

typedef struct {
	int LinkID; // ·��id
	int SourceID; // ���id
	int DestinationID; // Ŀ���id
	int Cost; // ����
	float trial; // ��Ϣ��
	float smell[600]; // ��ζ��smell[i] ���� �ڵ�i ɢ������ζ�ڴ˱��ϵ�ǿ��
	bool enable; // �Ƿ���Ч
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
int path[RADIUS+1]; // ·����
double smell[20]; // ÿ��·������ζ
int pathLength;
//	short vertexIndex[600]; // ��������·��λ��
} SmellPath; // ��ζ����·��
*/
typedef struct {

	short id;

	Edge *edges[8]; // ���ȱ�
	int degree; // ����
	int retainDegree;

	Edge *in_edges[600]; // ��ȱ�
	int in_degree; // ���
	//	float trial; // ��Ϣ��
	//	double smell[600];
	//	int smellPath[100000]; // smellPath[i]:�ö�������ζ·��i�ϵ�����λ��
} Vertex;

typedef struct graph{
	Edge *edges[4800]; // �洢��
	int numOfE;
	Vertex *vertexs[600]; // �洢�ڵ�
	int numOfV;
	Edge *vTov[600][600]; // �㵽��֮��ı�
	int vTovCost[600][600];
	Circle *circle[5000];
	int numOfCircle;

	int SourceID; // ���
	int DestinationId; // �յ�
	bool IncludingSet[600]; // �Ƿ��Ǳؾ��ڵ�
	int demand[50]; // �洢�ؾ��ڵ�
	int numOfDemand; // �ؾ��ڵ�����
	int vDemandId[600];

	int path[600];
	int pathLength;
	bool IncludingPathSet[600];
	int currentCost;
	int crossNum;

	int minCost; // ��ǰ��С����
	int minPath[600]; // ��С����·��
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
	int index_id[4800];
	int id_index[4800];
} Graph;

// ��ͼ
Graph* readGraph(const char *graphFile, const char *pathFile); 

// ��ȡʱ��
long getTime(); 

// ����·����Ч��
bool check(int path[600], int pathLength, Graph *graph); 

bool checkCircle(Graph *graph, const double *result, int length);
void preDelCircle(short k, short src, Graph *graph);
