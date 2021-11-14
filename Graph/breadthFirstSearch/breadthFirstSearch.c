
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Queue.h"

int visited[100];
void breadthFirstSearch(Graph g, int src) {
	int num = GraphNumVertices(g);
	for (int i = 0; i < num; i++) {
		visited[i] = -1;
	}
	visited[src] = src;
	Queue Q = QueueNew();
	QueueEnqueue(Q, src);
	while (!QueueIsEmpty(Q)) {
		int v = QueueDequeue(Q);
		for (int w = 0; w < num; w++) {
			if (visited[w] == -1 && GraphIsAdjacent(g,v,w) == true) {
				visited[w] = w;
				QueueEnqueue(Q, w);
				printf("%d ", w);
			}
		}
	}
}