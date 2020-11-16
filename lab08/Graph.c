#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

//	adding locations into array-Path
static void InsertPath (Vertex num, int *path) 
{
	int i = 0;
	while (path[i] != -1) i++;
	path[i] = num;
}


//
static int PathCheckBFS (Graph g, Vertex src, Vertex dest, int max, int *path, int len) 
{
	int visit[100] = {0}, after[100] = {0}, found = 0;
	for (int c = 0; c < g->nV; c++) visit[c] = -1;
	Queue q = newQueue();

	// Add src
	Vertex to = 0, curr = src;
	visit[curr] = curr;
	QueueJoin(q, curr);

	while (!QueueIsEmpty(q) && found == 0) {
		curr = QueueLeave(q);

		if (curr == dest) {
			found = 1;
		} else {
			for (to = 0; to < g->nV; to++) {
				// for every to in nV, if adjacent and unvisited, then add to q.
				if (g->edges[curr][to] > 0 && g->edges[curr][to] <= max && visit[to] == -1) {
					visit[to] = curr;
					QueueJoin(q, to);
				}
			}
		}
	}

	// get length and insert numbers into path.
	if (found == 1) {
		// len initially set as 1, cur starts from dest.
		Vertex cur = dest, tmp = cur;
		len = 1;	
		
		// len added from the src to dest, insert after for every num.
		while (cur != src) 
		{
			len++;
			tmp = cur;
			cur = visit[cur];
			after[cur] = tmp;
		}

		// InsertPath from src to dest
		InsertPath(src, path);
		while (cur != dest) 
		{
			cur = after[cur];
			InsertPath(cur, path);
		}
	}

	dropQueue(q);
	return len;
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);

	int len = 0;
	for (int i = 0; i < g->nV; i++) path[i] = -1;
	
	// if there is no edge between src and dest, then make edge.
	if (g->edges[src][dest] == 0) {
		mkEdge(g, src, dest);
		len = 0;
	}
	
	len = PathCheckBFS(g, src, dest, max, path, len);

	return len;
}

