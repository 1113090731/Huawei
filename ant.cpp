#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ant.h"

// ����Ϊ��ʼ״̬
void reset(Ant *ant){

	ant->path.init(ant->graph);
	ant->path.addVertex(ant->graph->SourceID);

}
// ��������
Ant *createAnt(Graph *graph){
	Ant *ant = new Ant();
	ant->graph = graph;
	reset(ant);
	return ant;
}
// �õ����������
double rnd(double dbLow, double dbUpper)
{
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp*(dbUpper - dbLow);
}

void search(Ant *ant){

	reset(ant);

	Graph *graph = ant->graph;

	while (true){

		// �������յ�
		if (ant->path.end == graph->DestinationId){
			// ���Ҿ����������м��
			if (ant->path.crossNum == graph->numOfDemand){
				// ��ʱ�ҵ�һ������·��
				//printf("find a path!!!!!!!!!!!!!!!! cost:%d time:%dms\n", ant->cost, getTime() - graph->startTime);
				// ������ڵ�ǰ���ţ���������
				if (ant->path.cost < graph->minCost){
					graph->minCost = ant->path.cost;
					//graph->minPath.copy(ant->path);
				}
				// ������Ϣ��
				/*
				for (int i = 1; i < graph->minpathLength; i++){
					Edge *edge = graph->vTov[graph->minPath[i - 1]][graph->minPath[i]];
					if (edge->trial<2000)
						edge->trial += 50000 / graph->minCost;
				}
				*/
				// ֹͣ����
				return;
			}
			// ����ʱδ���������м��
			// ����������ʧ��
			//printf("not all inside!\n");
			return;
		}


		int currentVertexID = ant->path.end;
		Vertex *vertex = graph->vertexs[currentVertexID];

		double sum = 0;
		double *prob = new double[vertex->degree];

		for (int i = 0; i < vertex->degree; i++){

			Edge *edge = vertex->edges[i];
			// ѡ·
			if (edge != NULL &&edge->enable&& !ant->path.inPath[edge->DestinationID] && ant->path.cost + vertex->edges[i]->Cost < graph->minCost){
				// �����յ㵫��û�������бؾ��㣬�����߲�ѡ
				if (edge->DestinationID == graph->DestinationId && ant->path.crossNum != graph->numOfDemand)
					prob[i] = 0;
				else{
					prob[i] = powf(edge->trial, graph->ALPHA);
					float beta = 0.001;
					// �ӱؾ�����յ����ζ
					for (int j = 0; j < 600; j++){
						if (graph->IncludingSet[j] || j == graph->SourceID){
							// û���߹��ıؾ��������ζ
							if (!ant->path.inPath[j]){
								// ȡ��ǿ����ζ
								if (edge->smell[j]>beta)
									beta = edge->smell[j];
							}
							else
								beta += 0.001;
						}
						else if (j == graph->DestinationId){
							// �߹����бؾ����յ������ζ
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
			// ��·����
			//printf("no path!\n");
			// ������Ϣ��
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

		//����ѡ��
		double dbTemp = rnd(0, sum); //ȡһ�������
		int selectedEdge = 0;
		for (int i = 0; i < vertex->degree; i++)
		{
			dbTemp = dbTemp - prob[i]; //��������൱��ת������
			if (dbTemp < 0.0) //����ֹͣת�������³��б�ţ�ֱ������ѭ��
			{
				selectedEdge = i;
				break;
			}
		}

		// ��¼��·��
		// vertex->edges[selectedEdge]Ϊѡ�еı�
		int targetVertexId = vertex->edges[selectedEdge]->DestinationID;
		ant->path.addVertex(targetVertexId);
		delete[] prob;
	}
}