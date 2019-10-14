// Copyright 2019 Genome Research Limited

#ifndef AGP_GRAPH_H
#define AGP_GRAPH_H

#include <stddef.h>
#include "agp_file.h"

typedef struct _AGPNode AGPNode;

struct _AGPNode {
    const char* id;
    char orientation;
    agp_line* entry;

    size_t left_n;
    size_t right_n;
    AGPNode** left;
    AGPNode** right;
};

#endif // defined AGP_GRAPH_H
