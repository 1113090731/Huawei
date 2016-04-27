
#pragma once

#include "graph.h"
#include "path.h"

struct HeapElement{
	ID vId;
	int cost;
};

struct MinHeap{

	Graph *graph;
	HeapElement *heap[MAX_INDEX+1];
	ID id_heapId[MAX_INDEX];
	int n;

	MinHeap(Graph *g){
		n = 0;
		graph = g;
	}

	void swap(int p1, int p2){
		HeapElement *tmp = heap[p1];
		heap[p1] = heap[p2];
		heap[p2] = tmp;

		id_heapId[heap[p1]->vId] = p1;
		id_heapId[heap[p2]->vId] = p2;
	}

	void siftUp(int index){
		int k = index;
		while (k > 1){
			if (heap[k]->cost < heap[k / 2]->cost){
				swap(k,k/2);
			}
			else{
				break;
			}
			k /= 2;
		}
	};

	void siftDown(int index){
		int k = index;
		while (k * 2 <= n){
			k *= 2;
			if (k < n && heap[k]->cost > heap[k + 1]->cost) {
				++k;
			}
			if (heap[k]->cost < heap[k / 2]->cost) {
				swap(k, k / 2);
			}
			else {
				break;
			}
		}
	};

	void insert(HeapElement *element){
		heap[++n] = element;
		id_heapId[element->vId] = n;
		siftUp(n);
	};

	void decrease(int index){
		int x = heap[index]->cost;
		int y = heap[n]->cost;
		n -= 1;

		// 若删除节点位于最末位置，则删除成功，无需其他操作。   
		if (index == n + 1)
			return;

		heap[index] = heap[n + 1];
		if (y >= x) {
			siftDown(index);
		}
		else {
			siftUp(index);
		}
	};

	HeapElement* decreaseMin(){
		HeapElement *e = heap[1];
		decrease(1);
		return e;
	};
};

struct MinHeapForPath{

	Graph *graph;
	Path_ *heap[MAX_INDEX + 1];
	ID id_heapId[MAX_INDEX];
	int n;

	MinHeapForPath(Graph *g){
		n = 0;
		graph = g;
	}

	void swap(int p1, int p2){
		Path_ *tmp = heap[p1];
		heap[p1] = heap[p2];
		heap[p2] = tmp;
	}

	void siftUp(int index){
		int k = index;
		while (k > 1){
			if (heap[k]->cost < heap[k / 2]->cost){
				swap(k, k / 2);
			}
			else{
				break;
			}
			k /= 2;
		}
	};

	void siftDown(int index){
		int k = index;
		while (k * 2 <= n){
			k *= 2;
			if (k < n && heap[k]->cost > heap[k + 1]->cost) {
				++k;
			}
			if (heap[k]->cost < heap[k / 2]->cost) {
				swap(k, k / 2);
			}
			else {
				break;
			}
		}
	};

	void insert(Path_ *element){
		heap[++n] = element;
		siftUp(n);
	};

	void decrease(int index){
		int x = heap[index]->cost;
		int y = heap[n]->cost;
		n -= 1;

		// 若删除节点位于最末位置，则删除成功，无需其他操作。   
		if (index == n + 1)
			return;

		heap[index] = heap[n + 1];
		if (y >= x) {
			siftDown(index);
		}
		else {
			siftUp(index);
		}
	};

	Path_* decreaseMin(){
		if (n == 0)
			return NULL;
		Path_ *e = heap[1];
		decrease(1);
		return e;
	};
};

