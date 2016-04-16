
#pragma once

#include "graph.h"
#include "CONST.h"
#include <string.h>

class Path_{
public:
	Path_(Graph *graph){
		this->graph = graph;
		init(graph);
	}
	Path_(){
		this->graph = NULL;
		init();
	}

	void init(Graph *graph = NULL);
	void print();
	void addVertex(int vId);
	void addVertexBack(int vId);
	void reveal();
	void pop();
	void copy(Path_ &src);

	int start, end, cost, crossNum;
	bool inPath[MAX_NUM_V];
	int path[MAX_NUM_V];
	int length;
private:
	Graph *graph;
};