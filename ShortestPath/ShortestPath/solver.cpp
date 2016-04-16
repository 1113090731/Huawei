
#include "graph.h"
#include "solver.h"
#include <vector>

void loadProblem(OsiClpSolverInterface *solver, Graph *graph){

	int *index_id = graph->index_id;
	// 目标函数
	double *obj = new double[graph->numOfE];
	int ei = 0;
	for (int i = 0; i < 4800; i++){
		if (graph->edges[i] != NULL){
			index_id[ei] = i;
			graph->id_index[i] = ei;
			obj[ei++] = graph->edges[i]->Cost;
		}
	}
	// 约束个数 src+dst+mid+d+dt+ctrl
	int indexOfRow = 0;
	int numrows = 3 + graph->numOfDemand + (graph->numOfV - 2) * 2;// +graph->numOfCircle;
	int name_index[7000];
	int SRC = 0;
	int DST = 1;
	int COS = 2;
	int MID = 3;
	int D = 603;
	int DT = 1203;
	int CRC = 1803;
	// 变量个数
	int numcols = graph->numOfE;

	// 变量上下界
	double *collb = new double[numcols];
	double *colub = new double[numcols];
	for (int i = 0; i < numcols; i++){
		collb[i] = 0; colub[i] = 1;
	}
	// 约束上下界
	double *rowlb = new double[numrows];
	double *rowub = new double[numrows];
	name_index[SRC] = indexOfRow;
	rowlb[0] = 1; rowub[0] = 1;
	indexOfRow++;
	name_index[DST] = indexOfRow;
	rowlb[1] = 1; rowub[1] = 1;
	indexOfRow++;
	name_index[COS] = indexOfRow;
	rowlb[2] = 0; rowub[2] = 96000;
	indexOfRow++;
	for (int i = 0; i < graph->numOfDemand; i++){
		name_index[MID + graph->demand[i]] = indexOfRow;
		rowlb[indexOfRow] = 1; rowub[indexOfRow] = 1;
		indexOfRow++;
	}
	for (int i = 0; i < 600; i++){
		if (graph->vertexs[i] != NULL && i != graph->SourceID && i != graph->DestinationId){
			//fprintf(file, " E D%d\n", i);
			name_index[D + i] = indexOfRow;
			rowlb[indexOfRow] = 0; rowub[indexOfRow] = 0;
			indexOfRow++;
			//fprintf(file, " L DT%d\n", i);
			name_index[DT + i] = indexOfRow;
			rowlb[indexOfRow] = 0; rowub[indexOfRow] = 1;
			indexOfRow++;
		}
	}
	for (int i = 0; i < graph->numOfCircle; i++){
		//fprintf(file, " L Crcl%d\n", graph->circle[i]->CircleID);
//		name_index[CRC + graph->circle[i]->CircleID] = indexOfRow;
//		rowlb[indexOfRow] = 0; rowub[indexOfRow] = graph->circle[i]->numOfV - 1;
//		indexOfRow++;
	}

	// 填充稀疏矩阵值
	int *start = new int[graph->numOfE + 1];
	std::vector<int> index;
	std::vector<double> values;
	int eIndex = 0;
	for (int i = 0; i < graph->numOfE; i++){
		Edge *edge = graph->edges[index_id[i]];
		
		start[eIndex++] = index.size();
		index.push_back(name_index[COS]);
		values.push_back(edge->Cost);
		if (edge->SourceID == graph->SourceID){
			index.push_back(name_index[SRC]);
			values.push_back(1);
		}
		else{
			index.push_back(name_index[D + edge->SourceID]);
			values.push_back(-1);
		}
		if (edge->DestinationID == graph->DestinationId){
			index.push_back(name_index[DST]);
			values.push_back(1);
		}
		else{
			index.push_back(name_index[D + edge->DestinationID]);
			values.push_back(1);
			index.push_back(name_index[DT + edge->DestinationID]);
			values.push_back(1);
		}
		if (graph->IncludingSet[edge->DestinationID]){
			index.push_back(name_index[MID + edge->DestinationID]);
			values.push_back(1);
		}
		for (int i = 0; i < edge->numOfCircle; i++){
//			index.push_back(name_index[CRC + edge->circle[i]]);
//			values.push_back(1);
		}
	}
	start[graph->numOfE] = index.size();
	int *array_index = new int[index.size()];
	double *array_value = new double[values.size()];
	for (int i = 0; i < index.size(); i++){
		array_index[i] = index[i];
		array_value[i] = values[i];
	}

	solver->loadProblem(numcols, numrows, start, array_index, array_value,
		collb, colub, obj, rowlb, rowub);

	for (int i = 0; i < numcols; i++)
		solver->setInteger(i);
}