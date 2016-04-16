
#include "KSP.h"
#include "MinHeap.h"
#include <iostream>

PathList* KSP(Graph *g, int start, int end, int k){
	PathList *result = new PathList();
	PathList *backup = new PathList();
	// 先找最短路径
	Path *path = new Path(); path->deviation_edge = -1;
	path->cost = dijkstra(g, start, end, path->path, &path->end);

	if (path->cost == MAXINT){
		return result;
	}
	backup->push(path);

	int num = 0;
	while (true)
	{
		if (backup->empty())
			break;
		Path *cur = backup->top(); backup->pop();
		result->push(cur); num++;
		if (num == k)
			break;

		bool *disable = new bool[g->numOfV];
		for (int i = 0; i < g->numOfV; i++)
			disable[i] = false;

		for (int j = cur->end - 1; j >= cur->deviation_src; j--){
			int sum = 0;
			Path *_path = new Path();
			_path->disable_edge[g->vTov[cur->path[j]][cur->path[j + 1]]->LinkID] = true;

			for (int i = 0; i < 4800; i++){
				if (cur->disable_edge[i])
					_path->disable_edge[i] = cur->disable_edge[i];
			}

			for (int z = 0; z < j; z++){
				disable[cur->path[z]] = true;
				sum += g->vTovCost[cur->path[z]][cur->path[z + 1]];
				_path->path[z] = cur->path[z];
			}

			_path->cost = sum + dijkstra(g, cur->path[j], end, &_path->path[j], &_path->end, disable, _path->disable_edge);
			_path->end += j;
			_path->deviation_src = j;
			if (_path->cost < MAXINT)
				backup->push(_path);

			for (int i = 0; i < g->numOfV; i++)
				disable[i] = false;
		}
		delete[] disable;
	}
	return result;
}

PathList* KSPwithCount(Graph *g, int start, int end, int k,bool *used){

	bool _used[600];
	if (used != NULL){
		for (int i = 0; i < 600; i++)
			_used[i] = used[i];
	}
	else{
		for (int i = 0; i < 600; i++)
			_used[i] = false;
	}

	PathList *result = new PathList();
	PathList *backup = new PathList();
	// 先找最短路径
	Path *path = new Path(); path->deviation_edge = -1;
	path->cost = dijkstra(g, start, end, path->path, &path->end, _used);

	if (path->cost == MAXINT){
		return result;
	}
	backup->push(path);

	int num = -1;
	int maxCost = -1;
	while (true)
	{
		if (backup->empty())
			break;
		Path *cur = backup->top(); backup->pop();
		
		if (cur->cost > maxCost){
			num++;
			maxCost = cur->cost;
		}
		if (num == k)
			break;
		result->push(cur);
		bool *disable = new bool[g->numOfV];
		for (int i = 0; i < g->numOfV; i++)
			disable[i] = _used[i];

		for (int j = cur->end - 1; j >= cur->deviation_src; j--){
			int sum = 0;
			Path *_path = new Path();
			_path->disable_edge[g->vTov[cur->path[j]][cur->path[j + 1]]->LinkID] = true;

			for (int i = 0; i < 4800; i++){
				if (cur->disable_edge[i])
					_path->disable_edge[i] = cur->disable_edge[i];
			}

			for (int z = 0; z < j; z++){
				disable[cur->path[z]] = true;
				sum += g->vTovCost[cur->path[z]][cur->path[z + 1]];
				_path->path[z] = cur->path[z];
			}

			_path->cost = sum + dijkstra(g, cur->path[j], end, &_path->path[j], &_path->end, disable, _path->disable_edge);
			_path->end += j;
			_path->deviation_src = j;
			if (_path->cost < MAXINT)
				backup->push(_path);

			for (int i = 0; i < g->numOfV; i++)
				disable[i] = _used[i];
		}
		delete[] disable;
	}
	return result;
}

PathList* KSPeqValue(Graph *g, int start, int end, Path *path,int value, bool *used, int disEdgeId){

	bool _used[600], _disEdge[4800];
	if (used != NULL){
		for (int i = 0; i < 600; i++)
			_used[i] = used[i];
	}
	else{
		for (int i = 0; i < 600; i++)
			_used[i] = false;
	}

	PathList *result = new PathList();
	PathList *backup = new PathList();

	backup->push(path);

	while (true)
	{
		if (backup->empty())
			break;
		Path *cur = backup->top(); backup->pop();

		if (cur->cost > value){
			break;
		}
		if (cur!=path)
			result->push(cur);
		bool *disable = new bool[g->numOfV];
		for (int i = 0; i < g->numOfV; i++)
			disable[i] = _used[i];

		for (int j = cur->end - 1; j >= cur->deviation_src; j--){
			int sum = 0;
			Path *_path = new Path();
			_path->disable_edge[g->vTov[cur->path[j]][cur->path[j + 1]]->LinkID] = true;

			for (int i = 0; i < 4800; i++){
				if (cur->disable_edge[i])
					_path->disable_edge[i] = cur->disable_edge[i];
			}
			if (disEdgeId != -1)
				_path->disable_edge[disEdgeId] = true;

			for (int z = 0; z < j; z++){
				disable[cur->path[z]] = true;
				sum += g->vTovCost[cur->path[z]][cur->path[z + 1]];
				_path->path[z] = cur->path[z];
			}

			_path->cost = sum + dijkstra(g, cur->path[j], end, &_path->path[j], &_path->end, disable, _path->disable_edge);
			_path->end += j;
			_path->deviation_src = j;
			if (_path->cost < MAXINT)
				backup->push(_path);

			for (int i = 0; i < g->numOfV; i++)
				disable[i] = _used[i];
		}
		delete[] disable;
	}
	return result;
}

PathList* KSPeqValue(Graph *g, int start, int end, int value, bool *used, int disEdgeId){

		bool _used[600], _disEdge[4800];
		if (used != NULL){
			for (int i = 0; i < 600; i++)
				_used[i] = used[i];
		}
		else{
			for (int i = 0; i < 600; i++)
				_used[i] = false;
		}

		PathList *result = new PathList();
		PathList *backup = new PathList();
		// 先找最短路径
		Path *path = new Path(); path->deviation_edge = -1;
		path->cost = dijkstra(g, start, end, path->path, &path->end, _used,disEdgeId);

		if (path->cost == MAXINT){
			return result;
		}
		backup->push(path);

		while (true)
		{
			if (backup->empty())
				break;
			Path *cur = backup->top(); backup->pop();

			if (cur->cost > value){
				break;
			}

			result->push(cur);
			bool *disable = new bool[g->numOfV];
			for (int i = 0; i < g->numOfV; i++)
				disable[i] = _used[i];

			for (int j = cur->end - 1; j >= cur->deviation_src; j--){
				int sum = 0;
				Path *_path = new Path();
				_path->disable_edge[g->vTov[cur->path[j]][cur->path[j + 1]]->LinkID] = true;

				for (int i = 0; i < 4800; i++){
					if (cur->disable_edge[i])
						_path->disable_edge[i] = cur->disable_edge[i];
				}
				if (disEdgeId != -1)
					_path->disable_edge[disEdgeId] = true;

				for (int z = 0; z < j; z++){
					disable[cur->path[z]] = true;
					sum += g->vTovCost[cur->path[z]][cur->path[z + 1]];
					_path->path[z] = cur->path[z];
				}

				_path->cost = sum + dijkstra(g, cur->path[j], end, &_path->path[j], &_path->end, disable, _path->disable_edge);
				_path->end += j;
				_path->deviation_src = j;
				if (_path->cost < MAXINT)
					backup->push(_path);

				for (int i = 0; i < g->numOfV; i++)
					disable[i] = _used[i];
			}
			delete[] disable;
		}
		return result;
	}

PathList* KSPwithCountValue(Graph *g, int start, int end, int k, int value,bool *used){

	bool _used[600];
	if (used != NULL){
		for (int i = 0; i < 600; i++)
			_used[i] = used[i];
	}
	else{
		for (int i = 0; i < 600; i++)
			_used[i] = false;
	}

	PathList *result = new PathList();
	PathList *backup = new PathList();
	// 先找最短路径
	Path *path = new Path(); path->deviation_edge = -1;
	path->cost = dijkstra(g, start, end, path->path, &path->end, _used);

	if (path->cost == MAXINT){
		return result;
	}
	backup->push(path);

	int num = 0;
	int maxCost = value;
	while (true)
	{
		if (backup->empty())
			break;
		Path *cur = backup->top(); backup->pop();

		if (cur->cost > maxCost){
			break;
		}
		if (num == k)
			break;
		result->push(cur);
		bool *disable = new bool[g->numOfV];
		for (int i = 0; i < g->numOfV; i++)
			disable[i] = _used[i];

		for (int j = cur->end - 1; j >= cur->deviation_src; j--){
			int sum = 0;
			Path *_path = new Path();
			_path->disable_edge[g->vTov[cur->path[j]][cur->path[j + 1]]->LinkID] = true;

			for (int i = 0; i < 4800; i++){
				if (cur->disable_edge[i])
					_path->disable_edge[i] = cur->disable_edge[i];
			}

			for (int z = 0; z < j; z++){
				disable[cur->path[z]] = true;
				sum += g->vTovCost[cur->path[z]][cur->path[z + 1]];
				_path->path[z] = cur->path[z];
			}

			_path->cost = sum + dijkstra(g, cur->path[j], end, &_path->path[j], &_path->end, disable, _path->disable_edge);
			_path->end += j;
			_path->deviation_src = j;
			if (_path->cost < MAXINT)
				backup->push(_path);

			for (int i = 0; i < g->numOfV; i++)
				disable[i] = _used[i];
		}
		delete[] disable;
	}
	return result;
}

int dijkstra(Graph *g, int start, int end, int *path, int *pos){
	unsigned i;
	int _N = g->numOfV;

	MinHeap *minHeap = new MinHeap(g);
	int *preVertex = new int[_N];
	int *dist = new int[_N];
	bool *used = new bool[_N];
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
	if (start!=end)
		used[start] = true;		           // label the start node
	used[end] = false;

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
		}
		if (min_node == end) break;

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

	int mdist = MAXINT;
	if ( end < _N)
		mdist = dist[end];

	if (mdist != MAXINT){
		int cur_index = 0;
		path[cur_index] = end;
		while (preVertex[path[cur_index]] != -1){
			path[cur_index + 1] = preVertex[path[cur_index]];
			cur_index++;
		}
		path[cur_index + 1] = start;
		cur_index++;
		*pos = cur_index;
	}

	for (i = 0; i < (*pos + 1) / 2; i++){
		int tmp = path[i];
		path[i] = path[*pos - i];
		path[*pos - i] = tmp;
	}

	delete minHeap;
	delete[] used;
	delete[] dist;
	delete[] preVertex;

	return mdist;// == INFINITY ? -1 : mdist;
}

int dijkstra(Graph *g, int start, int end, int *path, int *pos, bool *disable){
	unsigned i;
	int _N = g->numOfV;

	MinHeap *minHeap = new MinHeap(g);
	int *preVertex = new int[_N];
	int *dist = new int[_N];
	bool *used = new bool[_N];
	// 初始化
	for (i = 0; i<_N; i++)
	{
		preVertex[i] = -1;
		used[i] = disable[i];
		dist[i] = g->vTovCost[start][i];

		HeapElement *e = new HeapElement();
		e->vId = i;
		e->cost = dist[i];
		minHeap->insert(e);
	}
	if (start != end)
		used[start] = true;		           // label the start node
	used[end] = false;

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
		}
		if (min_node == end) break;

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

	int mdist = MAXINT;
	if ( end < _N)
		mdist = dist[end];

	if (dist[end] != MAXINT){
		int cur_index = 0;
		path[cur_index] = end;
		while (preVertex[path[cur_index]] != -1){
			path[cur_index + 1] = preVertex[path[cur_index]];
			cur_index++;
		}
		path[cur_index + 1] = start;
		cur_index++;
		*pos = cur_index;

		for (i = 0; i < (*pos + 1) / 2; i++){
			int tmp = path[i];
			path[i] = path[*pos - i];
			path[*pos - i] = tmp;
		}
	}

	delete[] used;
	delete[] dist;
	delete[] preVertex;

	return mdist;// == INFINITY ? -1 : mdist;
}

int dijkstra(Graph *g, int start, int end, int *path, int *pos, bool *disable, bool *disable_edge){
	unsigned i;
	int _N = g->numOfV;

	MinHeap *minHeap = new MinHeap(g);
	int *preVertex = new int[_N];
	int *dist = new int[_N];
	bool *used = new bool[_N];
	// 初始化
	for (i = 0; i<_N; i++)
	{
		preVertex[i] = -1;
		used[i] = disable[i];
		if (g->vTov[start][i] != NULL && !disable_edge[g->vTov[start][i]->LinkID])
			dist[i] = g->vTovCost[start][i];
		else
			dist[i] = MAXINT;

		HeapElement *e = new HeapElement();
		e->vId = i;
		e->cost = dist[i];
		minHeap->insert(e);
	}
	if (start != end)
		used[start] = true;		           // label the start node
	used[end] = false;

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
		}
		if (min_node == end) break;

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

	int mdist = MAXINT;
	if (end < _N)
		mdist = dist[end];

	if (dist[end] != MAXINT){
		int cur_index = 0;
		path[cur_index] = end;
		while (preVertex[path[cur_index]] != -1){
			path[cur_index + 1] = preVertex[path[cur_index]];
			cur_index++;
		}
		path[cur_index + 1] = start;
		cur_index++;
		*pos = cur_index;

		for (i = 0; i < (*pos + 1) / 2; i++){
			int tmp = path[i];
			path[i] = path[*pos - i];
			path[*pos - i] = tmp;
		}
	}

	delete[] used;
	delete[] dist;
	delete[] preVertex;

	return mdist;// == INFINITY ? -1 : mdist;
}

int dijkstra(Graph *g, int start, int end, int *path, int *pos, bool *disable, int disable_edgeId){
	unsigned i;
	int _N = g->numOfV;

	MinHeap *minHeap = new MinHeap(g);
	int *preVertex = new int[_N];
	int *dist = new int[_N];
	bool *used = new bool[_N];
	// 初始化
	for (i = 0; i<_N; i++)
	{
		preVertex[i] = -1;
		used[i] = disable[i];
		if (g->vTov[start][i] != NULL && g->vTov[start][i]->LinkID != disable_edgeId)
			dist[i] = g->vTovCost[start][i];
		else
			dist[i] = MAXINT;

		HeapElement *e = new HeapElement();
		e->vId = i;
		e->cost = dist[i];
		minHeap->insert(e);
	}
	if (start != end)
		used[start] = true;		           // label the start node
	used[end] = false;

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
		}
		if (min_node == end) break;

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

	int mdist = MAXINT;
	if (end < _N)
		mdist = dist[end];

	if (dist[end] != MAXINT){
		int cur_index = 0;
		path[cur_index] = end;
		while (preVertex[path[cur_index]] != -1){
			path[cur_index + 1] = preVertex[path[cur_index]];
			cur_index++;
		}
		path[cur_index + 1] = start;
		cur_index++;
		*pos = cur_index;

		for (i = 0; i < (*pos + 1) / 2; i++){
			int tmp = path[i];
			path[i] = path[*pos - i];
			path[*pos - i] = tmp;
		}
	}

	delete[] used;
	delete[] dist;
	delete[] preVertex;

	return mdist;// == INFINITY ? -1 : mdist;
}