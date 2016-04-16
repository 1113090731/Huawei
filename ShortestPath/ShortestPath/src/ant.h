#pragma once

#include "graph.h"
#include "path.h"

typedef struct{

	Path_ path;

	Graph *graph; // ȫ������ָ��
} Ant;

Ant *createAnt(Graph *graph);
void search(Ant *ant);