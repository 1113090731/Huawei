
#include "stdio.h"
#include "search_test.h"
#include "MinHeap.h"
#include "KSP.h"
#include "path.h"

// 判断可达性
MinHeapForPath* accessable(Graph *g, int start,bool *disable = NULL){
	unsigned i;
	int _N = g->numOfV;

	MinHeap *minHeap = new MinHeap(g);
	
	int *preVertex = new int[_N];
	int *dist = new int[_N];
	bool *used = new bool[_N];
	int crossNum = 0;
	// 初始化
	for (i = 0; i<_N; i++)
	{
		preVertex[i] = -1;
		if (disable==NULL)
			used[i] = false;
		else
			used[i] = disable[i];
		if (used[i] && g->IncludingSet[i]){
			crossNum++;
		}
			

		dist[i] = g->vTovCost[start][i];
		if (used[i])
			dist[i] = MAXINT;

		HeapElement *e = new HeapElement();
		e->vId = i;
		e->cost = dist[i];
		minHeap->insert(e);
	}
	used[start] = true;		           // label the start node

	unsigned count = 0;
	while (count++ < _N)
	{
		int min_node = -1;
		int min_dist = MAXINT;

		/* Select */
		HeapElement *e = minHeap->decreaseMin();
		if (used[e->vId]) continue;
		min_dist = e->cost;
		min_node = e->vId;

		if (min_dist == MAXINT)break;

		/* Record shortest path from the current node to start node */
		if (min_node >= 0)
		{
			used[min_node] = true;
			dist[min_node] = min_dist;
			if (g->IncludingSet[min_node])
				crossNum++;
			if (crossNum == g->numOfDemand)
				break;
		}

		/* Adjust */
		for (i = 0; i<g->vertexs[min_node]->degree; i++)
		{
			int v_id = g->vertexs[min_node]->edges[i]->DestinationID;
			if (used[v_id])continue;  // ignore the used node

			int w = g->vTovCost[min_node][v_id]; //_array[min_node][i] < 0.0 ? INFINITY : _array[min_node][i];
			if (min_dist + w < dist[v_id])
			{
				dist[v_id] = min_dist + w;
				preVertex[v_id] = min_node;

				int heapId = minHeap->id_heapId[v_id];
				HeapElement *e = minHeap->heap[heapId];
				e->cost = dist[v_id];
				minHeap->siftUp(heapId);
			}
		}
	}

	MinHeapForPath *minHeapForPath = NULL;
	if (g->numOfDemand == crossNum){
		minHeapForPath = new MinHeapForPath(g);

		for (int i = 0; i < _N; i++){
			if (!g->IncludingSet[i])
				continue;

			if (i==start)
				continue;

			Path_ *path = new Path_(g);
			int end = i;
			int mdist = MAXINT;
		
			if (end < _N)
				mdist = dist[end];

			if (mdist != MAXINT){
				int cur_index = 0;
				path->addVertexBack(end);
				while (preVertex[path->end] != -1){
					path->addVertexBack(preVertex[path->end]);
					cur_index++;
				}
				path->addVertexBack(start);
			}
			path->reveal();
			if (path->cost!=0)
				minHeapForPath->insert(path);
		}
	}
	

	delete minHeap;
	delete[] used;
	delete[] dist;
	delete[] preVertex;

	return minHeapForPath;
}

void search_test(Graph *graph){
	

	SearchNode *stack[50];
	int stackLength = 0;
	MinHeapForPath* pathHeap = accessable(graph, graph->SourceID);
	stack[stackLength++] = new SearchNode(pathHeap);

	while (true){
		Path_ *cur = stack[stackLength - 1]->path;
		//cur->print();
		if (cur->crossNum == graph->numOfDemand){

			int path[600];
			int end;
			dijkstra(graph, cur->end, graph->DestinationId, path, &end,cur->inPath);
			if (end > 0){	
				for (int i = 1; i <= end; i++)
					cur->addVertex(path[i]);
				if (check(cur->path, cur->length, graph));
					//cur->print();
				break;
			}
			else{
				while (!stack[stackLength - 1]->nextPath()){
					stackLength--;
				}
				continue;
			}	
		}
			
		MinHeapForPath* _pathHeap = accessable(graph, cur->end, cur->inPath);

		if (_pathHeap == NULL){
			while (!stack[stackLength - 1]->nextPath()){
				stackLength--;
			}
			continue;
		}
		
		SearchNode *newNode = new SearchNode(stack[stackLength - 1], _pathHeap);
		stack[stackLength++] = newNode;
	}
	/*
	if (pathHeap!=NULL){
		printf("Accessable!\n");
		while (pathHeap->n!=0)
		{
			pathHeap->decreaseMin()->print();
		}
	}
	else{
		printf("unAccessable!\n");
	}
	*/
}