
#include <Windows.h>
#include <stdio.h>
#include "graph.h"
#include "path.h"
#include "KSP.h"

// Windows下获取时间
long getTime() {
	LARGE_INTEGER frequency, count;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&count);
	long t = count.QuadPart;
	double fre = frequency.QuadPart;
	return 1000* (t / fre);
}
// 检测结果
bool check(ID *path, int pathLength, Graph *graph){
	bool inPath[600];
	for (int i = 0; i < 600; i++)
		inPath[i] = false;
	int num = 0;
	int sum = 0;
	for (int i = 0; i < pathLength - 1; i++){
		if (graph->vTov[path[i]][path[i + 1]] == NULL){
			printf("no edge %d-%d", path[i], path[i+1]);
			return false;
		}
		sum += graph->vTov[path[i]][path[i + 1]]->Cost;
		if (inPath[path[i]]){
			printf("copy vertex ");
			return false;
		}
		inPath[path[i]] = true;
		if (graph->IncludingSet[path[i]])
			num++;
	}
	printf("Cost %d ", sum);
	if (num != graph->numOfDemand){
		printf("not all instead %d",num);
		return false;
	}
	return true;
}
// KSP路径气味
void buildKSP(short k, short src, short dst, Graph *graph){
	Path_ *path = dijkstra(graph, src, dst, graph->IncludingSet);
	if (path == NULL)
		return;
	for (int i = 0; i < path->length-1; i++){
		Edge *edge = graph->vTov[path->path[i]][path->path[i + 1]];
		edge->smell[dst] += 2;
	}
}
//void buildKSP(short k, short src, short dst, Graph *graph){
//	PathList *list = KSP(graph, src, dst, k);
//	while (!list->empty()){
//		Circle *c = new Circle();
//		c->CircleID = graph->numOfCircle;
//		graph->circle[graph->numOfCircle++] = c;
//		for (int i = 0; i < list->top()->end; i++){
//			Edge *edge = graph->vTov[list->top()->path[i]][list->top()->path[i + 1]];
//			edge->smell[dst] += 1;
//			c->path[c->numOfV++] = list->top()->path[i];
//		}
//		c->path[c->numOfV++] = list->top()->path[list->top()->end];
//		list->pop();
//	}
//}
// 范围气味
void genSmell(Graph *graph, Vertex *vertex, int smellTag, int distance, int *path, int length, short *vIndex){

	bool hasSubPath = false;
	for (int i = 0; i < vertex->in_degree; i++){
		Edge *edge = vertex->in_edges[i];

		int vId = edge->SourceID;
		if (vId<0)
			continue;


		if (vIndex[vId]>0)
			continue;

		if (edge->Cost + distance <= RADIUS){
			hasSubPath = true;
			distance += edge->Cost;
			path[length] = vId;
			vIndex[vId] = length++;
			genSmell(graph, graph->vertexs[vId], smellTag, distance, path, length, vIndex);
			distance -= edge->Cost;
			length--;
			vIndex[vId] = 0;
		}
	}

	if (!hasSubPath){
		//SmellPath *smellPath = new SmellPath();
		//memcpy(smellPath->path, path, length*sizeof(int));
		//memcpy(smellPath->vertexIndex, vIndex, 600*sizeof(short));
		//smellPath->pathLength = length;
		//smellPath->smellTag = smellTag;
		//smellPath->id = graph->numOfSmellPath;
		//graph->numOfSmellPath++;
		//graph->smellPath[graph->numOfSmellPath++] = smellPath;

		int pathDistance = 0;
		int j = 0;
		for (; j < length - 1; j++){
			//graph->vertexs[path[j]]->smellPath[smellPath->id] = j+1;

			Edge *edge = graph->vTov[path[j + 1]][path[j]];
			pathDistance += edge->Cost;
			//smellPath->smell[j] = 1.0 / pathDistance;
			//if (edge->smell[smellTag]<1.0 / pathDistance)	
			edge->smell[smellTag] += 1.0 / pathDistance;
			//edge->smell[smellTag] += 0.1;
		}
		//graph->vertexs[path[j]]->smellPath[smellPath->id] = j+1;
	}
}

void setDemandId(Graph *graph,ID i){
	graph->IncludingSet = graph->Including[i];
	graph->numOfDemand = graph->numOfDemands[i];
	graph->demand = graph->demands[i];
}

Graph* readGraph(const char *graphFile, const char *pathFile){

	Graph *graph = new Graph();
	graph->startTime = getTime();

	// 信息素启发因子
	graph->ALPHA = 2;
	// 期望启发因子
	graph->BETA = 3;
	// 残留系数
	// 1-ROU 为挥发系数
	graph->ROU = 0.9;

	graph->crossNum = 0;
	graph->currentCost = 0;
	graph->minCost = MAXINT;
	// 读路径数据
	FILE *pathData = fopen(pathFile, "rt");
	if (pathData == NULL)
		printf("File '%s' open failed\n", pathFile);
	for (int i = 0; i < 2; i++){
		fscanf(pathData, "%d,%d,%d,", &graph->SourceID, &graph->SourceID, &graph->DestinationId);
		int pathVertexId;
		graph->numOfDemands[i] = 0;

		while (!feof(pathData)){
			fscanf(pathData, "%d", &pathVertexId);
			graph->Including[i][pathVertexId] = true;
			graph->demands[i][graph->numOfDemands[i]++] = pathVertexId;
			graph->vDemandId[i][pathVertexId] = graph->numOfDemands[i];
			char c = fgetc(pathData);
			if (c == '\n')
				break;		
		}
	}
	//FILE *pathData = fopen(pathFile, "rt");
	//if (pathData == NULL)
	//	printf("File '%s' open failed\n", pathFile);
	//// 读起点和终点
	//fscanf(pathData, "%d,%d,", &graph->SourceID, &graph->DestinationId);

	//// 读经过顶点
	//int pathVertexId;
	//graph->numOfDemand = 0;
	//fscanf(pathData, "%d", &pathVertexId);
	//graph->IncludingSet[pathVertexId] = true;
	//graph->demand[graph->numOfDemand++] = pathVertexId;
	//graph->vDemandId[pathVertexId] = graph->numOfDemand;

	//int last = 0;
	//while (!feof(pathData)){
	//	fscanf(pathData, "|%d", &pathVertexId);
	//	if (pathVertexId == last)
	//		break;
	//	graph->IncludingSet[pathVertexId] = true;
	//	graph->demand[graph->numOfDemand++] = pathVertexId;
	//	graph->vDemandId[pathVertexId] = graph->numOfDemand;
	//	last = pathVertexId;
	//}

	fclose(pathData);

	// 读图数据
	FILE *graphData = fopen(graphFile, "rt");
	if (graphData == NULL)
		printf("File '%s' open failed\n", graphFile);

	while (!feof(graphData)){

		Edge *edge = new Edge();
		edge->enable = true;
		edge->trial = 1500;
		for (int i = 0; i < MAX_INDEX; i++)
			edge->smell[i] = 0.001;
		fscanf(graphData, "%d,%d,%d,%d\n", &edge->LinkID, &edge->SourceID, &edge->DestinationID, &edge->Cost);

		if (edge->SourceID == graph->DestinationId || edge->DestinationID == graph->SourceID || edge->SourceID == edge->DestinationID)
			continue;
		if (graph->vTov[edge->SourceID][edge->DestinationID] == NULL){
			graph->vTov[edge->SourceID][edge->DestinationID] = edge;
		}
		else if (edge->Cost < graph->vTov[edge->SourceID][edge->DestinationID]->Cost){
			graph->vTov[edge->SourceID][edge->DestinationID] = edge;
		}
		else{
			continue;
		}

		graph->edges[edge->LinkID] = edge;

		Vertex *src = graph->vertexs[edge->SourceID];
		if (src == NULL){
			Vertex *vertex = new Vertex();
			graph->vertexs[edge->SourceID] = vertex;
			src = vertex;
		}
		src->edges[src->degree++] = edge;

		Vertex *dst = graph->vertexs[edge->DestinationID];
		if (dst == NULL){
			Vertex *vertex = new Vertex();
			graph->vertexs[edge->DestinationID] = vertex;
			dst = vertex;
		}
		dst->in_edges[dst->in_degree++] = edge;
	}

	for (int i = 0; i < MAX_INDEX; i++){
		if (graph->vertexs[i] != NULL)
			graph->numOfV++;
	}

	for (int i = 0; i < MAX_EDGE; i++){
		if (graph->edges[i] != NULL)
			graph->numOfE++;
	}

	if (graph->vertexs[graph->DestinationId] == NULL)
		return graph;

	fclose(graphData);

	//long addPos = graph->numOfV;
	//for (int i = 0; i < addPos; i++){
	//	if (graph->SourceID == i)
	//		continue;
	//	if (!graph->IncludingSet[i] || graph->DestinationId == i){
	//		Vertex *vertex = new Vertex();
	//		graph->vertexs[graph->numOfV++] = vertex;
	//	}
	//}
	//int numOfAddE = 0;
	//Edge *edge1 = new Edge();
	//edge1->enable = true;
	//edge1->SourceID = graph->DestinationId;
	//edge1->DestinationID = addPos;
	//edge1->Cost = 0;
	//graph->vTov[graph->DestinationId][addPos] = edge1;
	//numOfAddE++;
	//for (int i = addPos+1; i < graph->numOfV; i++){
	//	Edge *edge = new Edge();
	//	edge->enable = true;
	//	edge->SourceID = i-1;
	//	edge->DestinationID = i;
	//	edge->Cost = 0;
	//	graph->vTov[edge->SourceID][edge->DestinationID] = edge;
	//	numOfAddE++;
	//}
	//Edge *edge2 = new Edge();
	//edge2->enable = true;
	//edge2->SourceID = graph->numOfV-1;
	//edge2->DestinationID = graph->SourceID;
	//edge2->Cost = 0;
	//graph->vTov[edge2->SourceID][edge2->DestinationID] = edge2;
	//numOfAddE++;
	//
	//int offset = 0;
	//for (int i = 0; i < addPos; i++){
	//	if (graph->DestinationId == i || graph->SourceID == i)
	//		continue;
	//	if (!graph->IncludingSet[i]){
	//		Edge *edge = new Edge();
	//		edge->enable = true;
	//		edge->SourceID = addPos + offset;
	//		edge->DestinationID = i;
	//		edge->Cost = 0;
	//		graph->vTov[edge->SourceID][edge->DestinationID] = edge;
	//		numOfAddE++;
	//		offset++;
	//		Edge *edge1 = new Edge();
	//		edge1->enable = true;
	//		edge1->SourceID = i;
	//		edge1->DestinationID = addPos + offset;
	//		edge1->Cost = 0;
	//		graph->vTov[edge1->SourceID][edge1->DestinationID] = edge1;
	//		numOfAddE++;
	//	}
	//}

	for (ID i = 0; i < MAX_INDEX; i++)
	for (ID j = 0; j < MAX_INDEX; j++){
		if (graph->vTov[i][j] == NULL || !graph->vTov[i][j]->enable)
			graph->vTovCost[i][j] = MAXINT;
		else
			graph->vTovCost[i][j] = graph->vTov[i][j]->Cost;
	}
	
	/*
	int vCount[600];
	for (int i = 0; i < 600; i++)
		vCount[i] = 0;
	printf("circle num %d\n",graph->numOfCircle);
	for (int i = 0; i < graph->numOfCircle; i++){
		Circle *c = graph->circle[i];
		printf("c_%d ", c->CircleID);
		for (int j = 0; j < c->numOfV; j++){
			int vId = c->path[j];
			if (graph->IncludingSet[vId])
				printf("%d^.", vId);
			else
				printf("%d.", vId);
			vCount[vId]++;
		}
		printf("\n");
	}
	for (int i = 0; i < 600; i++){
		if (vCount[i] != 0)
			printf("v_%d count %d\n",i,vCount[i]);
	}
	*/
	return graph;
}

//void preDelCircle(short k, short src, Graph *graph){
//	PathList *list = KSPwithCount(graph, src, src, k);
//
//	while (!list->empty()){
//		Circle *c = new Circle();
//		graph->circle[graph->numOfCircle] = c;
//		c->CircleID = graph->numOfCircle++;
//		printf("Circle%d %d", c->CircleID, list->top()->path[0]);
//		int cost = 0;
//		for (int i = 0; i < list->top()->end; i++){
//			Edge *edge = graph->vTov[list->top()->path[i]][list->top()->path[i + 1]];
//			cost += edge->Cost;
//			printf("-%d", list->top()->path[i + 1]);
//			c->edgeId[c->numOfEdge++] = graph->id_index[edge->LinkID];
//			edge->circle[edge->numOfCircle++] = c->CircleID;
//		}
//		list->pop();
//		printf(" Cost%d\n", cost);
//	}
//}
//
//void buildCircle(Graph *graph, Circle *circle){
//	
//	int src = 0;
//	for (int i = 0; i < circle->numOfDemand; i++){
//		int dst = circle->demandIndex[i];
//		Path *path = new Path(); 
//		for (int j = src; j < dst + 1; j++){
//			circle->inCircle[circle->path[j]] = false;
//			path->path[path->end++] = circle->path[j];
//		}
//		path->end--;
//		path->deviation_edge = -1;
//		path->cost = circle->demandCost[i];
//		PathList *list = KSPeqValue(graph, circle->path[src], circle->path[dst], path, circle->demandCost[i], circle->inCircle);
//		for (int j = src; j < dst + 1; j++)
//			circle->inCircle[circle->path[j]] = true;
//		while (!list->empty()){
//			Circle *c = new Circle();
//			graph->circle[graph->numOfCircle] = c;
//			c->CircleID = graph->numOfCircle++;
//			printf("Circle %d ", c->CircleID);
//			//printf("Circle %d ", c->CircleID);
//			int cost = 0;
//			//copy
//			while (c->numOfEdge < src){
//				c->edgeId[c->numOfEdge] = circle->edgeId[c->numOfEdge];
//				cost += graph->edges[graph->index_id[circle->edgeId[c->numOfEdge]]]->Cost;
//				printf("-%d", graph->edges[graph->index_id[circle->edgeId[c->numOfEdge]]]->SourceID);
//				c->numOfEdge++;
//			}
//			printf("(");
//			//build
//			for (int i = 0; i < list->top()->end; i++){
//				Edge *edge = graph->vTov[list->top()->path[i]][list->top()->path[i + 1]];
//				cost += edge->Cost;
//				printf("-%d", list->top()->path[i]);
//				c->edgeId[c->numOfEdge++] = graph->id_index[edge->LinkID];
//			}
//			printf("-%d", list->top()->path[list->top()->end]);
//			printf(")");
//			//copy
//			for (int j = dst; j < circle->numOfEdge; j++){
//				c->edgeId[c->numOfEdge++] = circle->edgeId[j];
//				cost += graph->edges[graph->index_id[circle->edgeId[j]]]->Cost;
//				printf("-%d", graph->edges[graph->index_id[circle->edgeId[j]]]->DestinationID);
//			}
//			printf(" Cost %d\n",cost);
//			list->pop();
//		}
//		src = dst;
//	}
//
//}
//
//bool checkCircle(Graph *graph, const double *result,int length){
//
//	int *index_id = graph->index_id;
//	for (int i = 0; i < length; i++){
//		if (abs(result[i] - 1) < 0.01)
//			graph->edges[index_id[i]]->selected = true;
//	}
//
//	graph->crossNum = 0;
//	graph->pathLength = 0;
//	int cur = graph->SourceID;
//	graph->path[graph->pathLength++] = cur;
//	graph->IncludingPathSet[cur] = true;
//	while (true){
//		if (cur == graph->DestinationId){
//			break;
//		}
//		Vertex *v = graph->vertexs[cur];
//		for (int i = 0; i < v->degree; i++){
//			Edge *edge = v->edges[i];
//			if (edge == NULL)
//				continue;
//			if (edge->selected){
//				cur = edge->DestinationID;
//				graph->path[graph->pathLength++] = cur;
//				graph->IncludingPathSet[cur] = true;
//				graph->currentCost += edge->Cost;
//				if (graph->IncludingSet[cur]){
//					graph->crossNum++;
//				}
//				break;
//			}
//		}
//	}
//	if (graph->crossNum == graph->numOfDemand){
//		printf("success!\n");
//		if (graph->currentCost < graph->minCost){
//			graph->minCost = graph->currentCost;
//			graph->minpathLength = graph->pathLength;
//			memcpy(graph->minPath, graph->path, graph->pathLength * sizeof(int));
//		}
//		//check(graph->path, graph->pathLength, graph);
//		graph->currentCost = 0;
//		graph->pathLength = 0;
//		graph->crossNum = 0;
//		for (int i = 0; i < length; i++){
//			if (abs(result[i] - 1) < 0.01)
//				graph->edges[index_id[i]]->selected = false;
//		}
//		for (int i = 0; i < 600; i++)
//			graph->IncludingPathSet[i] = false;
//		return true;
//	}
//	else{
//		printf("no all cross %d\n", graph->numOfDemand - graph->crossNum);
//	}
//
//	int lastCircle = graph->numOfCircle;
//	for (int i = 0; i < graph->numOfDemand; i++){
//		if (!graph->IncludingPathSet[graph->demand[i]]){
//			Circle *c = new Circle();
//			graph->circle[graph->numOfCircle] = c;
//			c->CircleID = graph->numOfCircle++; // 环id
//			printf("Circle %d ",c->CircleID);
//			int cur = graph->demand[i];
//			c->path[c->numOfV++] = cur; // 环路径(点)
//			printf("-%d", cur);
//			c->inCircle[cur] = true; // 点是否在路径上
//			graph->IncludingPathSet[cur] = true;
//			int n = 0;
//			while (true){
//				if (cur == graph->demand[i] && n != 0){
//					printf("\n");
//					break;
//				}
//				n++;
//				Vertex *v = graph->vertexs[cur];
//				for (int i = 0; i < v->degree; i++){
//					Edge *edge = v->edges[i];
//					if (edge == NULL)
//						continue;
//					if (edge->selected){
//						c->cost += edge->Cost;
//						c->demandCost[c->numOfDemand] += edge->Cost;
//						cur = edge->DestinationID;
//						c->edgeId[c->numOfEdge++] = graph->id_index[edge->LinkID]; // 环路径（边）
//						edge->circle[edge->numOfCircle++] = c->CircleID;
//						c->path[c->numOfV++] = cur;
//						c->inCircle[cur] = true;
//						printf("-%d", cur);
//						if (graph->IncludingPathSet[cur]){
//							//printf("copy v%d\n", edge->DestinationID);
//							break;
//						}				
//						graph->IncludingPathSet[cur] = true;
//						if (graph->IncludingSet[cur]){
//							graph->crossNum++;
//							printf("^");
//							c->demandIndex[c->numOfDemand++] = c->numOfV-1;
//						}
//						break;
//					}
//				}
//			}//end while
//		} 
//	}
//	/*
//	for (int i = lastCircle; i < graph->numOfCircle; i++){	
//		if (graph->circle[i]->cost>0){
//			printf("---build Circle %d Cost %d\n", graph->circle[i]->CircleID, graph->circle[i]->cost);
//			buildCircle(graph, graph->circle[i]);
//		}
//	}
//	*/
//	graph->currentCost = 0;
//	graph->pathLength = 0;
//	graph->crossNum = 0;
//	for (int i = 0; i < length; i++){
//		if (abs(result[i] - 1) < 0.01)
//			graph->edges[index_id[i]]->selected = false;
//	}
//	for (int i = 0; i < 600; i++)
//		graph->IncludingPathSet[i] = false;
//	return false;
//}