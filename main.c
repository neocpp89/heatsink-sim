#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"

const char default_nfname[] = "nodes.txt";

int main(int argc, char **argv)
{
    char *nfname;
    if (argc > 1) {
        nfname = argv[argc-1];
    } else {
        nfname = default_nfname;
    }
    write_log(0, "Using node file %s.\n", nfname);
    return 0;
}

