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

    size_t n_left;
    size_t n_right;
    AGPEdge** left;
    AGPEdge** right;
};

typedef struct _AGPGapTech {
    uint64_t length;
    int source_file_no;
} AGPGapTech;

struct _AGPEdge {
    AGPNode* out_path;
    agp_line* entry;
    size_t n_gap;
    AGPGapTech* gap;
};

int add_to_graph(const agp_file* restrict data);

#endif // defined AGP_GRAPH_H
