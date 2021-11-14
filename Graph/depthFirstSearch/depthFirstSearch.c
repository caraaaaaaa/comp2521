
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

void dfsPathCheck(Graph g, int v);

int visited[100];

void depthFirstSearch(Graph g, int src) {
	int num = GraphNumVertices(g);
	for (int i = 0; i < num; i++) {
		visited[i] = -1;
	}
	visited[src] = src;
	printf("%d ",src);
	dfsPathCheck(g,src);
}

void dfsPathCheck(Graph g,int v) {
	int num = GraphNumVertices(g);
	for (int w = 0; w < num ; w++) {	
		if (visited[w] == -1) {
			if (GraphIsAdjacent(g,v,w) == true) {
				visited[w] = w;
				printf("%d ",w);
				dfsPathCheck(g,w);
			}
		}
	}
}