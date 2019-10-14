// Copyright 2019 Genome Research Limited

#ifndef AGP_GRAPH_H
#define AGP_GRAPH_H

#include <stddef.h>
#include "agp_file.h"

typedef struct _AGPNode AGPNode;
typedef struct _AGPEdge AGPEdge;

struct _AGPNode {
    enum {CONTIG, START, END} type;
    const char* id;
    char orientation;
    agp_line* entry;

    size_t left_n;
    size_t right_n;
    AGPEdge** left;
    AGPEdge** right;
};

struct _AGPEdge {
    AGPNode* out_path;
    uint64_t length;
};

#endif // defined AGP_GRAPH_H
