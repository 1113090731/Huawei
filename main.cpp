
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ant.h"
#include "KSP.h"
#include "path.h"
#include "search_test.h"
#include "../atsp.h"

Path_ path;
Path_ minPath;
Path_ result[2];
int numOfResult = 0;
// 深度优先搜索
// 结果保存在minPath
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

void search_ant(Graph *graph){
	long start0 = getTime();
	Path_ *minPath = new Path_(graph);
	minPath->cost = MAXINT;
	Ant *ant[M];
	for (int i = 0; i < M; i++)
		ant[i] = createAnt(graph);
	int min = 96000;
	int count = 0;
	long total = 0;
	int n = 0;	
	while (true){

		for (int i = 0; i < M - n; i++){
			long t0 = getTime();
			search(ant[i]);
			if (ant[i]->path.end == graph->DestinationId && ant[i]->path.cost < minPath->cost){
				minPath->copy(ant[i]->path);
				check(minPath->path, minPath->length, graph);
				printf(" cross %d\n",minPath->crossNum);
			}
			count++;
			long t1 = getTime();
			total += (t1 - t0);
		}

		for (int i = 1; i < minPath->length; i++){
			Edge *edge = graph->vTov[minPath->path[i - 1]][minPath->path[i]];
			//if (edge->trial<2000)
			edge->trial += 50000 / minPath->cost;
		}

		for (int i = 1; i < MAX_EDGE; i++){
			Edge *edge = graph->edges[i];
			if (edge == NULL)
				continue;
			if (edge->trial>200)
				edge->trial *= graph->ROU;
		}

		long pastTime = getTime() - start0;

		if (pastTime > 2500)
			break;
	}
	minPath->print();
	result[numOfResult%2].copy(*minPath);
	numOfResult++;
}

int main(){

	Graph *graph = readGraph("test/cc0/topo.csv", "test/cc0/demand.csv");
	Graph *graph1 = readGraph("test/cc0/topo.csv", "test/cc0/demand.csv");
	Graph *graph2 = readGraph("test/cc0/topo.csv", "test/cc0/demand.csv");

	setDemandId(graph, 0);

	// 范围气味
	for (int i = 0; i < MAX_INDEX; i++){

		if (graph->IncludingSet[i] || graph->DestinationId == i){
			int path[RADIUS];
			short *vIndex = new short[MAX_INDEX];
			path[0] = i;
			vIndex[i] = 0;
			genSmell(graph, graph->vertexs[i], i, 0, path, 1, vIndex);
			delete[] vIndex;
		}
	}
	//
	for (int i = 0; i < MAX_INDEX; i++){
		if (graph->IncludingSet[i] || graph->DestinationId == i){
			for (int j = 0; j < MAX_INDEX; j++){
				if (i != j && (graph->IncludingSet[j] || graph->SourceID == j)){
					buildKSP(1, j, i, graph);
				}
			}
		}
	}

	long t0 = getTime();
	search_ant(graph);
	long t1 = getTime();
	printf("time %dms\n",t1-t0);

	for (int i = 0; i < result[0].length - 1; i++){
		int eId = graph->vTov[result[0].path[i]][result[0].path[i + 1]]->LinkID;
		graph1->edges[eId]->Cost += 20000;
	}

	setDemandId(graph1, 1);

	// 范围气味
	for (int i = 0; i < MAX_INDEX; i++){

		if (graph1->IncludingSet[i] || graph1->DestinationId == i){
			int path[RADIUS];
			short *vIndex = new short[MAX_INDEX];
			path[0] = i;
			vIndex[i] = 0;
			genSmell(graph1, graph->vertexs[i], i, 0, path, 1, vIndex);
			delete[] vIndex;
		}
	}
	//
	for (int i = 0; i < MAX_INDEX; i++){
		if (graph1->IncludingSet[i] || graph1->DestinationId == i){
			for (int j = 0; j < MAX_INDEX; j++){
				if (i != j && (graph1->IncludingSet[j] || graph1->SourceID == j)){
					buildKSP(1, j, i, graph1);
				}
			}
		}
	}

	long t2 = getTime();
	search_ant(graph1);
	long t3 = getTime();
	printf("time %dms\n", t3 - t2);

	for (int i = 0; i < result[1].length - 1; i++){
		int eId = graph->vTov[result[1].path[i]][result[1].path[i + 1]]->LinkID;
		graph2->edges[eId]->Cost += 20000;
	}

	setDemandId(graph2, 0);

	// 范围气味
	for (int i = 0; i < MAX_INDEX; i++){

		if (graph2->IncludingSet[i] || graph2->DestinationId == i){
			int path[RADIUS];
			short *vIndex = new short[MAX_INDEX];
			path[0] = i;
			vIndex[i] = 0;
			genSmell(graph2, graph2->vertexs[i], i, 0, path, 1, vIndex);
			delete[] vIndex;
		}
	}
	//
	for (int i = 0; i < MAX_INDEX; i++){
		if (graph2->IncludingSet[i] || graph2->DestinationId == i){
			for (int j = 0; j < MAX_INDEX; j++){
				if (i != j && (graph2->IncludingSet[j] || graph2->SourceID == j)){
					buildKSP(1, j, i, graph2);
				}
			}
		}
	}

	long t4 = getTime();
	search_ant(graph2);
	long t5 = getTime();
	printf("time %dms\n", t5 - t4);

	for (int i = 0; i < result[0].length - 1; i++){
		int eId = graph->vTov[result[0].path[i]][result[0].path[i + 1]]->LinkID;
		graph->edges[eId]->selected = true;
	}
	int crossNum = 0;
	for (int i = 0; i < result[1].length - 1; i++){
		int eId = graph->vTov[result[1].path[i]][result[1].path[i + 1]]->LinkID;
		if (graph->edges[eId]->selected){
			crossNum++;
		}		
	}
	printf("crossNum:%d  totalCost:%d\n", crossNum, (result[0].cost + result[1].cost) % 20000);
	return 0;
}


