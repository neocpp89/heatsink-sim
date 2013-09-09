#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "node.h"
#include "text_reader.h"

char default_nfname[] = "nodes.txt";

int main(int argc, char **argv)
{
    char *nfname;
    FILE *nf;
    node_t *nodes;
    int num_nodes;
    int i;

    if (argc > 1) {
        nfname = argv[argc-1];
    } else {
        nfname = default_nfname;
    }

    write_log(0, "Trying node file %s.\n", nfname);

    nf = fopen(nfname, "r");
    if (nf == NULL) {
        write_log(0, "Error opening node file %s!\n", nfname);
        return -1;
    } else {
        write_log(0, "Opened node file %s.\n", nfname);
    }
    read_nodes(&nodes, &num_nodes, nf);

    for (i = 0; i < num_nodes; i++) {
        printf("node id %d: x = %f, y = %f\n",
            nodes[i].id, nodes[i].x, nodes[i].y);
    }

    return 0;
}

