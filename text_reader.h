#include <stdio.h>
#include "node.h"

#ifndef __TEXT_READER_H_
#define __TEXT_READER_H_
void read_nodes(node_t **nodes, int *num_nodes, FILE *nodefile);
void read_thermal_props(node_t *nodes, int num_nodes, FILE *kappafile);
#endif

