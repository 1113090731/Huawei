
#pragma once

#include "graph.h"
#include "path.h"
#include "MinHeap.h"

struct SearchNode{

	SearchNode(MinHeapForPath *minHeap4path){
		this->minHeap4path = minHeap4path;
		Path_ *subPath = minHeap4path->decreaseMin();
		this->path = new Path_();
		this->path->copy(*subPath);
		delete subPath;
		parent = NULL;
	}

	SearchNode(const SearchNode *parent,MinHeapForPath *minHeap4path){
		this->parent = parent;
		this->minHeap4path = minHeap4path;
		this->path = new Path_();
		this->path->copy(*parent->path);
		Path_ *subPath = minHeap4path->decreaseMin();
		this->path->addPath(*subPath);
		delete subPath;
	}

	bool nextPath(){
		Path_ *subPath = minHeap4path->decreaseMin();
		if (subPath == NULL){
			return false;
		}
		delete this->path;
		this->path = new Path_();
		if (parent != NULL){
			this->path->copy(*parent->path);
			this->path->addPath(*subPath);
		}
		else
			this->path->copy(*subPath);
		delete subPath;
		return true;
	}
	const SearchNode *parent;
	MinHeapForPath *minHeap4path;
	Path_ *path;
} ;

void search_test(Graph *graph);