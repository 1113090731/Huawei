
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ant.h"
#include "KSP.h"
#include "path.h"
#include "search_test.h"

Path_ path;
Path_ minPath;
// �����������
// ���������graph->minPath
void search_dfs(Graph *graph){

	int currentVertexId = path.end;
	//printf("·������ %d\n",graph->pathLength);
	if (currentVertexId == graph->DestinationId){
		if (path.crossNum == graph->numOfDemand){
			// �����Ѿ��������е�
			graph->minCost = path.cost;
			minPath.copy(path);
		}
		//printf("�����յ�\n");
		return;
	}

	Vertex *vertex = graph->vertexs[currentVertexId];

	if (vertex == NULL){
		//printf("����Ϊ0\n");
		return;
	}

	for (int i = 0; i < vertex->degree; i++)
	{
		Edge *edge = vertex->edges[i];
		if (edge == NULL)
			continue;
		int DestinationID = edge->DestinationID;

		if (path.inPath[DestinationID]){
			//printf("��·\n");
			continue;
		}
		if (path.cost + edge->Cost > graph->minCost){

			//printf("����\n");
			continue;
		}

		path.addVertex(DestinationID);
		search_dfs(graph);
		path.pop();
	}

	return;
}

int main(){

	Graph *graph = readGraph("test/case1/topo.csv", "test/case1/demand.csv");
	
	search_test(graph);

	return 0;
}


