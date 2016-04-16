
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
// 深度优先搜索
// 结果保存在graph->minPath
void search_dfs(Graph *graph){

	int currentVertexId = path.end;
	//printf("路径长度 %d\n",graph->pathLength);
	if (currentVertexId == graph->DestinationId){
		if (path.crossNum == graph->numOfDemand){
			// 并且已经经过所有点
			graph->minCost = path.cost;
			minPath.copy(path);
		}
		//printf("到达终点\n");
		return;
	}

	Vertex *vertex = graph->vertexs[currentVertexId];

	if (vertex == NULL){
		//printf("出度为0\n");
		return;
	}

	for (int i = 0; i < vertex->degree; i++)
	{
		Edge *edge = vertex->edges[i];
		if (edge == NULL)
			continue;
		int DestinationID = edge->DestinationID;

		if (path.inPath[DestinationID]){
			//printf("死路\n");
			continue;
		}
		if (path.cost + edge->Cost > graph->minCost){

			//printf("费用\n");
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


