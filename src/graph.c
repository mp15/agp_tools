// Copyright 2019 Genome Research Limited
#include "agp_file.h"
#include "graph.h"
#include "khash.h"

#include <stddef.h>

KHASH_MAP_INIT_STR(str_node, AGPNode*)

struct _AGPGraph {
    AGPNode* start;
    kh_str_node_t* components;
};

static AGPNode* init_node(agp_line* restrict line)
{
    AGPNode* node = (AGPNode*)malloc(sizeof(AGPNode));
    node->left = NULL;
    node->right = NULL;
    node->n_left = 0;
    node->n_right = 0;
    node->entry = line;

    return node;
}

AGPGraph* init_graph()
{
    AGPGraph* graph = (AGPGraph*)malloc(sizeof(AGPGraph));
    graph->components = kh_init_str_node();

    return graph;
}

int add_to_graph(const agp_file* restrict data, AGPGraph* graph)
{
    static int source_file_no = 0;

    /*
    graph->start = (AGPNode*)malloc(sizeof(AGPNode));
    graph->start->type = START;
    graph->components = kh_init_str_node();
    AGPNode* last_node = graph->start;
     */
    // Now add it to the graph
    size_t i = 0;
    while ( i < data->n_records) {
        char* last_obj = data->lines[i]->object;
        if (data->lines[i]->component_type == known_gap || data->lines[i]->component_type == unknown_gap ) {
            fprintf(stderr, "Unexpected gap component\n");
            return 1;
        }
        AGPNode* last_node = NULL;
        AGPNode* node = NULL;
        AGPEdge* edge = NULL;
        khint_t ptr;
        // Is the component already in the graph
        if ( ( ptr = kh_get_str_node(graph->components, data->lines[i]->component_id )) == kh_end(graph->components) ) {
            last_node = node = init_node(data->lines[i]);

            int ret;
            ptr = kh_put_str_node(graph->components, data->lines[i]->component_id, &ret);
            if (ret == -1) { fprintf(stderr,"Unable to insert node into hash.\n"); return 1; }
            kh_value(graph->components, ptr) = node;
        } else {
            last_node = node = kh_value(graph->components, ptr);
        }
        // TODO: save this starting node
        ++i;

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
            edge->gap[0].source_file_no = source_file_no;
            
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
            if ( ( ptr = kh_get_str_node(graph->components, data->lines[i]->component_id )) == kh_end(graph->components) ) {
                node = init_node(data->lines[i]);

                int ret;
                ptr = kh_put_str_node(graph->components, data->lines[i]->component_id, &ret);
                if (ret == -1) { fprintf(stderr,"Unable to insert node into hash.\n"); return 1; }
                kh_value(graph->components, ptr) = node;
            } else {
                node = kh_value(graph->components, ptr);
            }
            edge->out_path = node;

            last_node = node;
            i++;
        }
    }

    source_file_no++;
    return 0;
}

void dump_graph(const AGPGraph* graph)
{
    printf("components: %d\n",kh_size(graph->components));
}
