#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ant.h"

// 设置为初始状态
void reset(Ant *ant){

	ant->path.init(ant->graph);
	ant->path.addVertex(ant->graph->SourceID);

}
// 创建蚂蚁
Ant *createAnt(Graph *graph){
	Ant *ant = new Ant();
	ant->graph = graph;
	reset(ant);
	return ant;
}
// 得到区间随机数
double rnd(double dbLow, double dbUpper)
{
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp*(dbUpper - dbLow);
}

void search(Ant *ant){

	reset(ant);

	Graph *graph = ant->graph;

	while (true){

		// 到达了终点
		if (ant->path.end == graph->DestinationId){
			// 并且经过了所有中间点
			if (ant->path.crossNum == graph->numOfDemand){
				// 此时找到一条可行路径
				//printf("find a path!!!!!!!!!!!!!!!! cost:%d time:%dms\n", ant->cost, getTime() - graph->startTime);
				// 如果优于当前最优，保存下来
				if (ant->path.cost < graph->minCost){
					graph->minCost = ant->path.cost;
					//graph->minPath.copy(ant->path);
				}
				// 更新信息素
				/*
				for (int i = 1; i < graph->minpathLength; i++){
					Edge *edge = graph->vTov[graph->minPath[i - 1]][graph->minPath[i]];
					if (edge->trial<2000)
						edge->trial += 50000 / graph->minCost;
				}
				*/
				// 停止搜索
				return;
			}
			// 若此时未经过所有中间点
			// 该蚂蚁任务失败
			//printf("not all inside!\n");
			return;
		}


		int currentVertexID = ant->path.end;
		Vertex *vertex = graph->vertexs[currentVertexID];

		double sum = 0;
		double *prob = new double[vertex->degree];

		for (int i = 0; i < vertex->degree; i++){

			Edge *edge = vertex->edges[i];
			// 选路
			if (edge != NULL &&edge->enable&& !ant->path.inPath[edge->DestinationID] && ant->path.cost + vertex->edges[i]->Cost < graph->minCost){
				// 到达终点但是没经过所有必经点，这条边不选
				if (edge->DestinationID == graph->DestinationId && ant->path.crossNum != graph->numOfDemand)
					prob[i] = 0;
				else{
					prob[i] = powf(edge->trial, graph->ALPHA);
					float beta = 0.001;
					// 加必经点和终点的气味
					for (int j = 0; j < 600; j++){
						if (graph->IncludingSet[j] || j == graph->SourceID){
							// 没有走过的必经点才有气味
							if (!ant->path.inPath[j]){
								// 取最强的气味
								if (edge->smell[j]>beta)
									beta = edge->smell[j];
							}
							else
								beta += 0.001;
						}
						else if (j == graph->DestinationId){
							// 走过所有必经点终点才有气味
							if (ant->path.crossNum >= graph->numOfDemand)
								beta += edge->smell[j];
							else
								beta += 0.001;
						}
					}
					prob[i] *= powf(beta, graph->BETA);
				}
			}
			else
				prob[i] = 0;
			sum += prob[i];
		}

		if (sum == 0){
			// 无路可走
			//printf("no path!\n");
			// 更新信息素
			/*
			for (int i = 1; i < ant->pathLength; i++){
				Edge *edge = graph->vTov[ant->path[i - 1]][ant->path[i]];
				if (edge->trial>100)
					edge->trial *= 0.98;
			}
			*/
			return;
		}
		//==============================================================================

		//轮盘选择
		double dbTemp = rnd(0, sum); //取一个随机数
		int selectedEdge = 0;
		for (int i = 0; i < vertex->degree; i++)
		{
			dbTemp = dbTemp - prob[i]; //这个操作相当于转动轮盘
			if (dbTemp < 0.0) //轮盘停止转动，记下城市编号，直接跳出循环
			{
				selectedEdge = i;
				break;
			}
		}

		// 记录到路径
		// vertex->edges[selectedEdge]为选中的边
		int targetVertexId = vertex->edges[selectedEdge]->DestinationID;
		ant->path.addVertex(targetVertexId);
		delete[] prob;
	}
}