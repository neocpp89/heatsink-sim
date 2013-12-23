#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <suitesparse/cs.h>

#include "logger.h"
#include "node.h"
#include "text_reader.h"

char default_nfname[] = "nodes.txt";
char default_kfname[] = "kappa.txt";
char default_ofstr[] = "output_%d.txt";

cs *build_stiffness_matrix(double **f, node_t *nodes, int num_nodes, float dt);

cs *build_stiffness_matrix(double **f, node_t *nodes, int num_nodes, float dt)
{
    int i, j;
    int lda, nnz;
    double k_vphalf, k_vnhalf;
    double k_hphalf, k_hnhalf;
    double pdx, ndx, pdy, ndy, dx, dy;
    cs *triplets;
    cs *kmat;

    lda = num_nodes;
    *f = (double *)malloc(sizeof(double) * lda);
    nnz = 5 * lda;
    triplets = cs_spalloc(lda, lda, nnz, 1, 1);
    printf("Allocated triplets and load vector.\n");

    for (i = 0; i < lda; i++) {
        if (nodes[i].neighbors[3] == -1) {
            cs_entry(triplets, i, i, 1);
            (*f)[i] = 100;
            continue;
        }

        if (nodes[i].neighbors[0] == -1 ||
            nodes[i].neighbors[1] == -1 ||
            nodes[i].neighbors[2] == -1) {
            cs_entry(triplets, i, i, 1);
            (*f)[i] = 0;
            continue;
        }

        k_hphalf = 0.5 * (nodes[i].kappa + nodes[nodes[i].neighbors[0]].kappa);
        k_vphalf = 0.5 * (nodes[i].kappa + nodes[nodes[i].neighbors[1]].kappa);
        k_hnhalf = 0.5 * (nodes[i].kappa + nodes[nodes[i].neighbors[2]].kappa);
        k_vnhalf = 0.5 * (nodes[i].kappa + nodes[nodes[i].neighbors[3]].kappa);

        pdx = nodes[nodes[i].neighbors[0]].x - nodes[i].x;
        ndx = nodes[i].x - nodes[nodes[i].neighbors[2]].x;
        dx = 0.5 * (pdx + ndx);

        pdy = nodes[nodes[i].neighbors[1]].y - nodes[i].y;
        ndy = nodes[i].y - nodes[nodes[i].neighbors[3]].y;
        dy = 0.5 * (pdy + ndy);

        cs_entry(triplets, i, i,
            (1 + (dt/dy) * (k_vphalf/pdy + k_vnhalf/ndy) +
                (dt/dx) * (k_hphalf/pdx + k_hphalf/ndx)));

        cs_entry(triplets, i, nodes[i].neighbors[0],
            (-(dt/dx) * (k_hphalf/pdx)));

        cs_entry(triplets, i, nodes[i].neighbors[1],
            (-(dt/dy) * (k_vphalf/pdy)));

        cs_entry(triplets, i, nodes[i].neighbors[2],
            (-(dt/dx) * (k_hnhalf/ndx)));

        cs_entry(triplets, i, nodes[i].neighbors[3],
            (-(dt/dy) * (k_vnhalf/ndy)));

        (*f)[i] = -1;
    }

    kmat = cs_compress(triplets);
    printf("Compressed triplets.\n");
    fflush(stdout);

    return kmat;
}

int main(int argc, char **argv)
{
    char *nfname;
    char *kfname;
    char ofname[256];
    FILE *nf;
    FILE *kf;
    FILE *of;
    node_t *nodes;
    int num_nodes;
    int i, j;
    double *f = NULL;
    double *u = NULL;
    float t, dt, tf;
    cs *kmat;

    t = 0;
    dt = 1e-2;
    tf = 0.1;

    if (argc > 1) {
        nfname = argv[argc-1];
    } else {
        nfname = default_nfname;
    }
    kfname = default_kfname;

    write_log(0, "Trying node file %s.\n", nfname);

    nf = fopen(nfname, "r");
    if (nf == NULL) {
        write_log(0, "Error opening node file %s!\n", nfname);
        return -1;
    } else {
        write_log(0, "Opened node file %s.\n", nfname);
    }
    read_nodes(&nodes, &num_nodes, nf);
    fclose(nf);


    kf = fopen(kfname, "r");
    if (kf == NULL) {
        write_log(0, "Error opening kappa file %s!\n", kfname);
        return -1;
    } else {
        write_log(0, "Opened kappa file %s.\n", kfname);
    }
    read_thermal_props(nodes, num_nodes, kf);
    fclose(kf);

    printf("%d nodes read.\n", num_nodes);
    for (i = 0; i < num_nodes; i++) {
        printf("node id %d: x = %f, y = %f, k = %f\n",
            nodes[i].id, nodes[i].x, nodes[i].y, nodes[i].kappa);
    }

    j = 0;
    while (t < tf) {
        kmat = build_stiffness_matrix(&f, nodes, num_nodes, dt);

        if (u == NULL) {
            for (i = 0; i < num_nodes; i++) {
                if (f[i] == -1) {
                    f[i] = 0;
                }
            }
        } else {
            for (i = 0; i < num_nodes; i++) {
                if (f[i] == -1) {
                    f[i] = u[i];
                }
            }
        }

        if (j == 0) {
            snprintf(ofname, sizeof(ofname) / sizeof(ofname[0]), default_ofstr, j);
            of = fopen(ofname, "w");
            fprintf(of, "%d %f\n", num_nodes, t);
            for (i = 0; i < num_nodes; i++) {
                fprintf(of, "%d %f %f %f\n", i, nodes[i].x, nodes[i].y, f[i]);
            }
        }

        if (!cs_lusol(0, kmat, f, 1e-12)) {
            printf("lusol error!\n");
        }
        if (u != NULL) {
            free(u);
        }
        u = f;
        for (i = 0; i < num_nodes; i++) {
            printf("T[%d] = %f\n", i, u[i]);
        }
        t += dt;
        j++;

        snprintf(ofname, sizeof(ofname) / sizeof(ofname[0]), default_ofstr, j);
        of = fopen(ofname, "w");
        fprintf(of, "%d %f\n", num_nodes, t);
        for (i = 0; i < num_nodes; i++) {
            fprintf(of, "%d %f %f %f\n", i, nodes[i].x, nodes[i].y, u[i]);
        }
    }

    return 0;
}

