#pragma once

#include "graph.h"
#include "path.h"

typedef struct{

	Path_ path;

	Graph *graph; // 全局数据指针
} Ant;

Ant *createAnt(Graph *graph);
void search(Ant *ant);