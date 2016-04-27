
#include "CONST.h"
#include "graph.h"
#include "path.h"
#include <stdio.h>
#include <Windows.h>

void Path_::init(void *graph){
	length = 0;
	cost = 0;
	crossNum = 0;
	start = -1;
	end = -1;
	if (graph!=NULL)
		this->graph = graph;
	memset(inPath, false, sizeof(bool)*MAX_NUM_V);
	memset(repeatCount, 0, sizeof(ID)*MAX_NUM_V);
}

void Path_::print(){
	Graph *graph = (Graph *) this->graph;
	printf("\nPath from v_%d to v_%d |-Cost %d CrossNum %d\n",start,end, cost,crossNum);
	printf("|-In Vertex:\n");
	printf("|-");
	for (int i = 0; i < length; i++){
		int vId = path[i];
		if (graph->IncludingSet[vId])
			printf("-%d^",vId);
		else
			printf("-%d", vId);
	}
	printf("\n");

	printf("|-In Edge:\n");
	printf("|--");
	for (int i = 0; i < length-1; i++){
		int eId = graph->vTov[path[i]][path[i + 1]]->LinkID;
		printf("%d|", eId);
	}
	printf("\n");

}

void Path_::addVertex(ID vId){
	Graph *graph = (Graph *) this->graph;
	// ���
	if (length == 0){
		path[length++] = vId;
		start = vId;
		end = vId;
		repeatCount[vId]++;
		return;
	}

	// �ظ���
	else if (inPath[vId]){
		if (canCopy){
			//repeatCount[vId]++;
		}
		else{
			printf("Repeat v_%d\n", path[length - 1], vId);
			getchar();
			exit(1);
		}
	}

	Edge *edge = graph->vTov[path[length-1]][vId];
	// ������·��
	if (edge == 0){
		printf("There is not edge from v_%d to v_%d\n", path[length - 1],vId);
		getchar();
		exit(1);
	}
	repeatCount[vId]++;
	end = vId;
	path[length++] = vId;
	cost += edge->Cost;
	inPath[vId] = true;
	if (graph->IncludingSet[vId])
		++crossNum;
}

// int start, end, cost, crossNum;
void Path_::addPath(const Path_ &target){
	for (int i = 1; i < target.length; i++){
		int vId = target.path[i];
		this->addVertex(vId);
	}
}

void Path_::addVertexBack(ID vId){
	Graph *graph = (Graph *) this->graph;
	// �յ�
	if (length == 0){
		path[length++] = vId;
		inPath[vId] = true;
		start = vId;
		end = vId;
		if (graph->IncludingSet[vId])
			++crossNum;
		return;
	}

	// �ظ���
	else if (inPath[vId]){
		printf("Repeat v_%d\n", path[length - 1], vId);
		getchar();
		exit(1);
	}

	Edge *edge = graph->vTov[vId][path[length-1]];
	// ������·��
	if (edge == 0){
		printf("There is not edge from v_%d to v_%d\n", vId, path[length - 1]);
		getchar();
		exit(1);
	}

	end = vId;
	path[length++] = vId;
	cost += edge->Cost;
	inPath[vId] = true;
	if (graph->IncludingSet[vId])
		++crossNum;
}

void Path_::reveal(){
	for (int i = 0; i < length / 2; i++){
		int tmp = path[i];
		path[i] = path[length -1 - i];
		path[length - 1 - i] = tmp;
	}
	start = path[0];
	end = path[length - 1];
}

void Path_::pop(){
	Graph *graph = (Graph *) this->graph;
	if (graph->IncludingSet[end]){
		crossNum--;
	}
	Edge *edge = graph->vTov[path[length - 2]][end];
	cost -= edge->Cost;
	inPath[end] = false;
	end = path[length - 2];
	length--;
}

void Path_::copy(const Path_ &src){
	graph = src.graph;
	start = src.start;
	end = src.end;
	cost = src.cost;
	length = src.length;
	crossNum = src.crossNum;
	memcpy(inPath, src.inPath, sizeof(bool)*600);
	memcpy(path, src.path, sizeof(int)*length);	
}