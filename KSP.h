
#pragma once

#include <queue>
#include "graph.h"

using namespace std;

struct Path{
	int path[MAXINDEX];
	int end; // Ä©¶Ë
	int cost;
	int deviation_src; // ·ÖÀëµã
	int deviation_edge; // ·ÖÀë±ß
	bool disable_edge[4800];
};

struct Path_cmp{
	bool operator ()(Path *a, Path *b){
		return a->cost > b->cost;
	}
};

typedef priority_queue<Path*, vector<Path*>, Path_cmp> PathList;

PathList* KSP(Graph *g, int start, int end, int k);
PathList* KSPwithCount(Graph *g, int start, int end, int k,bool *used=NULL);
PathList* KSPwithCountValue(Graph *g, int start, int end, int k, int value, bool *used = NULL);
PathList* KSPeqValue(Graph *g, int start, int end, int value, bool *used = NULL, int disEdgeId = -1);
PathList* KSPeqValue(Graph *g, int start, int end, Path *path, int value, bool *used = NULL, int disEdgeId = -1);


int dijkstra(Graph *g, int start, int end, int *path, int *pos);
int dijkstra(Graph *g, int start, int end, int *path, int *pos, bool *disable, bool *disable_edge);
int dijkstra(Graph *g, int start, int end, int *path, int *pos, bool *disable, int disable_edgeId);
int dijkstra(Graph *g, int start, int end, int *path, int *pos, bool *disable);

