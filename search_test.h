
#pragma once

#include "graph.h"
#include "path.h"
#include "MinHeap.h"

struct SearchNode{

	SearchNode(MinHeapForPath *minHeap4path){
		this->minHeap4path = minHeap4path;
		this->path = minHeap4path->decreaseMin();
		parent = NULL;
	}

	SearchNode(const SearchNode *parent,MinHeapForPath *minHeap4path){
		this->parent = parent;
		this->minHeap4path = minHeap4path;
		this->path = new Path_();
		this->path->copy(*parent->path);
		this->path->addPath(*minHeap4path->decreaseMin());
	}

	bool nextPath(){
		Path_ *subPath = minHeap4path->decreaseMin();
		if (subPath == NULL){
			return false;
		}
		delete this->path;
		this->path = new Path_();
		this->path->copy(*parent->path);
		this->path->addPath(*subPath);
		return true;
	}
	const SearchNode *parent;
	MinHeapForPath *minHeap4path;
	Path_ *path;
} ;

void search_test(Graph *graph);