
#include <Windows.h>
#include <stdio.h>
#include "graph.h"
#include "KSP.h"

// Windows�»�ȡʱ��
long getTime() {
	LARGE_INTEGER frequency, count;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&count);
	long t = count.QuadPart;
	double fre = frequency.QuadPart;
	return 1000* (t / fre);
}
// �����
bool check(int path[600], int pathLength, Graph *graph){
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
		printf("not all instead ");
		return false;
	}
	return true;
}
// KSP·����ζ
void buildKSP(short k, short src, short dst, Graph *graph){
	PathList *list = KSP(graph, src, dst, k);
	while (!list->empty()){
		Circle *c = new Circle();
		c->CircleID = graph->numOfCircle;
		graph->circle[graph->numOfCircle++] = c;
		for (int i = 0; i < list->top()->end; i++){
			Edge *edge = graph->vTov[list->top()->path[i]][list->top()->path[i + 1]];
			edge->smell[dst] += 1;
			c->path[c->numOfV++] = list->top()->path[i];
		}
		c->path[c->numOfV++] = list->top()->path[list->top()->end];
		list->pop();
	}
}
// ��Χ��ζ
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

Graph* readGraph(const char *graphFile, const char *pathFile){

	Graph *graph = new Graph();
	graph->startTime = getTime();

	// ��Ϣ����������
	graph->ALPHA = 2;
	// ������������
	graph->BETA = 3;
	// ����ϵ��
	// 1-ROU Ϊ�ӷ�ϵ��
	graph->ROU = 0.9;

	graph->crossNum = 0;
	graph->currentCost = 0;
	graph->minCost = 96000;
	// ��·������
	FILE *pathData = fopen(pathFile, "rt");
	if (pathData == NULL)
		printf("File '%s' open failed\n", pathFile);
	// �������յ�
	fscanf(pathData, "%d,%d,", &graph->SourceID, &graph->DestinationId);

	// ����������
	int pathVertexId;
	graph->numOfDemand = 0;
	fscanf(pathData, "%d", &pathVertexId);
	graph->IncludingSet[pathVertexId] = true;
	graph->demand[graph->numOfDemand++] = pathVertexId;
	graph->vDemandId[pathVertexId] = graph->numOfDemand;

	int last = 0;
	while (!feof(pathData)){
		fscanf(pathData, "|%d", &pathVertexId);
		if (pathVertexId == last)
			break;
		graph->IncludingSet[pathVertexId] = true;
		graph->demand[graph->numOfDemand++] = pathVertexId;
		graph->vDemandId[pathVertexId] = graph->numOfDemand;
		last = pathVertexId;
	}

	fclose(pathData);

	// ��ͼ����
	FILE *graphData = fopen(graphFile, "rt");
	if (graphData == NULL)
		printf("File '%s' open failed\n", graphFile);

	while (!feof(graphData)){

		Edge *edge = new Edge();
		edge->enable = true;
		edge->trial = 1500;
		for (int i = 0; i < 600; i++)
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

	for (int i = 0; i < 600; i++){
		if (graph->vertexs[i] != NULL)
			graph->numOfV++;
	}

	for (int i = 0; i < 4800; i++){
		if (graph->edges[i] != NULL)
			graph->numOfE++;
	}

	if (graph->vertexs[graph->DestinationId] == NULL)
		return graph;

	fclose(graphData);

	/*
	// ��Χ��ζ
	for (int i = 0; i < 600; i++){

		if (graph->IncludingSet[i] || graph->DestinationId == i){
			int path[RADIUS];
			short *vIndex = new short[600];
			path[0] = i;
			vIndex[i] = 0;
			genSmell(graph, graph->vertexs[i], i, 0, path, 1, vIndex);
			delete[] vIndex;
		}
	}
	*/
	for (int i = 0; i < N; i++)
	for (int j = 0; j < N; j++){
		if (graph->vTov[i][j] == NULL || !graph->vTov[i][j]->enable)
			graph->vTovCost[i][j] = MAXINT;
		else
			graph->vTovCost[i][j] = graph->vTov[i][j]->Cost;
	}
	
	
	for (int i = 0; i < 600; i++){
		if (graph->IncludingSet[i] || graph->DestinationId == i){
			for (int j = 0; j < 600; j++){
				if (i != j && (graph->IncludingSet[j] || graph->SourceID == j)){
					buildKSP(1, j, i, graph);
				}
			}
		}
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
//			c->CircleID = graph->numOfCircle++; // ��id
//			printf("Circle %d ",c->CircleID);
//			int cur = graph->demand[i];
//			c->path[c->numOfV++] = cur; // ��·��(��)
//			printf("-%d", cur);
//			c->inCircle[cur] = true; // ���Ƿ���·����
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
//						c->edgeId[c->numOfEdge++] = graph->id_index[edge->LinkID]; // ��·�����ߣ�
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