// Copyright 2019 Genome Research Limited

#ifndef AGP_GRAPH_H
#define AGP_GRAPH_H

#include <stddef.h>

typedef struct _AGPNode AGPNode;

struct _AGPNode {
    const char* id;
    const char* contig_id;
    char orientation;

    size_t left_n;
    size_t right_n;
    AGPNode** left;
    AGPNode** right;
};

#endif // defined AGP_GRAPH_H