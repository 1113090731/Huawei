
#pragma once

//#include "graph.h"
#include "CONST.h"
#include <string.h>

class Path_{
public:
	Path_(void *graph){
		this->graph = graph;
		init(graph);
	}
	Path_(const Path_ &src){
		this->copy(src);
	}
	Path_(){
		this->graph = NULL;
		init();
	}

	void init(void *graph = NULL);
	void print();
	void addVertex(ID vId);
	void Path_::addVertexCanCopy(ID vId);
	void addVertexBack(ID vId);
	void reveal();
	void pop();
	void copy(const Path_ &src);
	void addPath(const Path_ &target);

	ID start, end;
	int cost, crossNum;
	bool inPath[MAX_NUM_V];
	ID path[MAX_NUM_V];
	bool canCopy;
	ID repeatCount[MAX_NUM_V];
	int length;
	int trail;

	void *graph;
};