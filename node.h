#ifndef __NODE_H_
#define __NODE_H_
typedef struct node_s {
    int id;
    float x;
    float y;
    /* 0 = east, 1 = north, 2 = west, 3 = south, stored by id. */
    int neighbors[4];
} node_t;
#endif

