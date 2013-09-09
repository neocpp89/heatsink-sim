#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "text_reader.h"

#define BUFFERSIZE 1024

void read_nodes(node_t **nodes, int *num_nodes, FILE *nodefile)
{
    int i;
    char line[BUFFERSIZE];    /* allocate a largeish buffer for fgets */

    if (fgets(line, BUFFERSIZE, nodefile) == NULL) {
        return;
    }
    sscanf(line, "%d", num_nodes);
    *nodes = (node_t *)malloc(*num_nodes * sizeof(node_t));

    for (i = 0; i < *num_nodes; i++) {
        if (fgets(line, BUFFERSIZE, nodefile) == NULL) {
            break;
        }
        sscanf(line, "%d %f %f %d %d %d %d",
            &((*nodes)[i].id),
            &((*nodes)[i].x),
            &((*nodes)[i].y),
            &((*nodes)[i].neighbors[0]),
            &((*nodes)[i].neighbors[1]),
            &((*nodes)[i].neighbors[2]),
            &((*nodes)[i].neighbors[3]));
    }

    /* we should probably build an index to id map, but for now assume
        index = id. */

    return;
}

