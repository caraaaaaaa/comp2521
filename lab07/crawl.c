#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"
#include "graph.h"
#include "html.h"
#include "set.h"
#include "stack.h"
#include "url_file.h"

#define BUFSIZE 1024

static void setFirstURL (char *, char *);

int main (int argc, char **argv)
{	
//	Declares
	URL_FILE *handle;
	char buffer[BUFSIZE], baseURL[BUFSIZE], firstURL[BUFSIZE], next[BUFSIZE];
	int maxURLs;

//	Examine or Check
	if (argc > 2) {
		strcpy (baseURL, argv[1]);
		setFirstURL (baseURL, firstURL);
		maxURLs = atoi (argv[2]);
		if (maxURLs < 40) maxURLs = 40;

	} else {
		fprintf (stderr, "Usage: %s BaseURL MaxURLs\n", argv[0]);
		exit (1);

	}

	// Add firstURL to the ToDo list
	Queue ToDo = newQueue();
	enterQueue(ToDo, firstURL);

	// Initialise Graph to hold up to maxURLs
	Graph map  = newGraph((size_t)maxURLs);

	// Initialise set of Seen URLs
	Set set = newSet();

	// While (ToDo list not empty and Graph not filled)
	while (emptyQueue(ToDo) == 0 && (nVertices(map) != (size_t)maxURLs)) {
		// grab Next URL from ToDo list
		char *nextURL = leaveQueue(ToDo);

		// if (not allowed) continues
		if (strstr(nextURL, "unsw") == NULL)  continue;

		if (!(handle = url_fopen (nextURL, "r"))) {
			fprintf (stderr, "Couldn't open %s\n", next);
			exit (1);
		}

		// foreach line in the opened URL
		while (!url_feof (handle)) {
			url_fgets (buffer, sizeof (buffer), handle);
			int pos = 0;
			char result[BUFSIZE];
			memset (result, 0, BUFSIZE);

			// foreach URL on that line
			while ((pos = GetNextURL(buffer, nextURL, result, pos)) > 0) {
				// if Graph not filled or both URLs in Graph
				// add an edge from Next to this URL
				if ((nVertices(map) != (size_t)maxURLs) || (isElem(set, nextURL) && isElem(set, result)))
					addEdge(map, nextURL, result);
			
				// if this URL not Seen already
				// add it to the Seen set and ToDo list
				if (!isElem(set, nextURL) || !isElem(set, result)) {
					printf ("Found: '%s'\n", result);
					enterQueue(ToDo, result);
					insertInto(set, result);
				}

				memset (result, 0, BUFSIZE);
			}
		}

		// close the opened URL
		url_fclose (handle);
		sleep (1);

		free(nextURL);
	}
    
    showGraph(map, 0);
    showGraph(map, 1);
	dropGraph(map);
	dropQueue(ToDo);
	dropSet(set);

	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
static void setFirstURL (char *base, char *first)
{
	char *c;
	if ((c = strstr (base, "/index.html")) != NULL) {
		strcpy (first, base);
		*c = '\0';

	} else if (base[strlen (base) - 1] == '/') {
		strcpy (first, base);
		strcat (first, "index.html");
		base[strlen (base) - 1] = '\0';

	} else {
		strcpy (first, base);
		strcat (first, "/index.html");

	}
}

