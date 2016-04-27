
#pragma once

#include <queue>
#include "graph.h"
#include "path.h"

using namespace std;

struct Path{
	ID path[MAX_INDEX];
	ID end; // Ä©¶Ë
	int cost;
	int deviation_src; // ·ÖÀëµã
	int deviation_edge; // ·ÖÀë±ß
	bool disable_edge[MAX_EDGE];
};

struct Path_cmp{
	bool operator ()(Path *a, Path *b){
		return a->cost > b->cost;
	}
};

typedef priority_queue<Path*, vector<Path*>, Path_cmp> PathList;

PathList* KSP(Graph *g, ID start, ID end, int k);
PathList* KSPwithCount(Graph *g, ID start, ID end, int k, bool *used = NULL);
PathList* KSPwithCountValue(Graph *g, ID start, ID end, int k, int value, bool *used = NULL);
PathList* KSPeqValue(Graph *g, ID start, ID end, int value, bool *used = NULL, int disEdgeId = -1);
PathList* KSPeqValue(Graph *g, ID start, ID end, Path *path, int value, bool *used = NULL, int disEdgeId = -1);


int dijkstra(Graph *g, ID start, ID end, ID *path, ID *pos);
int dijkstra(Graph *g, ID start, ID end, ID *path, ID *pos, bool *disable);
int dijkstra(Graph *g, ID start, ID end, ID *path, ID *pos, bool *disable, bool *disable_edge);
int dijkstra(Graph *g, ID start, ID end, ID *path, ID *pos, bool *disable, int disable_edgeId);


Path_* dijkstra(Graph *g, ID start, ID end, bool *disable = NULL);

