
#include "stdio.h"
#include "search_test.h"
#include "MinHeap.h"
#include "KSP.h"
#include "path.h"

// 判断可达性
MinHeapForPath* accessable(Graph *g, int start){
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
		used[i] = false;
		dist[i] = g->vTovCost[start][i];

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
	
	MinHeapForPath* pathHeap = NULL;
	long t0 = getTime();
	pathHeap = accessable(graph, graph->SourceID);
	long t1 = getTime();

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
	printf("time %dms\n",t1-t0);
	
}