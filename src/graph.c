// Copyright 2019 Genome Research Limited
#include "agp_file.h"
#include "graph.h"
#include "khash.h"

#include <stddef.h>

KHASH_MAP_INIT_STR(str_node, AGPNode*)

typedef struct _AGPGraph {
    AGPNode* start;
    kh_str_node_t* components;
} AGPGraph;

int add_to_graph(const agp_file* data)
{
    AGPGraph graph;

    /*
    graph.start = (AGPNode*)malloc(sizeof(AGPNode));
    graph.start->type = START;
    graph.components = kh_init_str_node();
    AGPNode* last_node = graph.start;
     */
    // Now add it to the graph
    size_t i = 0;
    while ( i < data->n_records) {
        char* last_obj = data->lines[i]->object;
        printf("Adding scaffold %s\n", data->lines[i]->object);
        if (data->lines[i]->component_type == known_gap || data->lines[i]->component_type == unknown_gap ) {
            fprintf(stderr, "Unexpected gap component\n");
            return 1;
        }
        AGPNode* last_node = NULL;
        AGPNode* node = NULL;
        AGPEdge* edge = NULL;
        khint_t ptr;
        // Is the component already in the graph
        if ( ( ptr = kh_get_str_node(graph.components, data->lines[i]->component_id )) == kh_end(graph.components) ) {
            last_node = node = (AGPNode*)malloc(sizeof(AGPNode));
            node->entry = data->lines[i];
            int ret;
            ptr = kh_put_str_node(graph.components, data->lines[i]->component_id, &ret);
            if (ret != 0) { fprintf(stderr,"Unable to insert node into hash.\n"); return 1; }
            kh_value(graph.components, ptr) = node;
        } else {
            last_node = node = kh_value(graph.components, ptr);
        }
        // TODO: save this starting node

        while (i < data->n_records && strcmp(last_obj,data->lines[i]->object) == 0) {
            if (data->lines[i]->component_type != known_gap && data->lines[i]->component_type != unknown_gap ) {
                fprintf(stderr, "Unexpected non-gap component\n");
                return 1;
            }
            // TODO:Check if last_node already has a path to current node
            edge = (AGPEdge*)malloc(sizeof(AGPEdge));
            edge->n_gap = 1;
            edge->gap = (AGPGapTech*)malloc(sizeof(AGPGapTech));
            edge->gap[0].length = data->lines[i]->gap_length;
            edge->gap[0].source_file_no = 0;
            
            last_node->right = realloc(last_node->right,sizeof(AGPEdge*)*(last_node->n_right+1));
            last_node->right[last_node->n_right++] = edge;

            i++;
            if (i == data->n_records) {
                fprintf(stderr, "Terminated in gap component\n");
                return 1;
            }
            if (data->lines[i]->component_type == known_gap || data->lines[i]->component_type == unknown_gap ) {
                fprintf(stderr, "Unexpected gap component\n");
                return 1;
            }
            // Is the component already in the graph
            if ( ( ptr = kh_get_str_node(graph.components, data->lines[i]->component_id )) == kh_end(graph.components) ) {
                node = (AGPNode*)malloc(sizeof(AGPNode));
                node->entry = data->lines[i];
                int ret;
                ptr = kh_put_str_node(graph.components, data->lines[i]->component_id, &ret);
                if (ret != 0) { fprintf(stderr,"Unable to insert node into hash.\n"); return 1; }
                kh_value(graph.components, ptr) = node;
            } else {
                node = kh_value(graph.components, ptr);
            }
            edge->out_path = node;

            last_node = node;
            i++;
        }
    }
    return 0;
}
