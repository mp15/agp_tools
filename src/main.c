// Copyright 2019 Genome Research Limited

#include "graph.h"
#include "agp_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int ch = 0;
    while ((ch = getopt(argc, argv, "bf:")) != -1) {
        switch (ch) {
        }
    }
    argc -= optind;
    argv += optind;

    if (argc < 1) {
        fprintf(stderr, "We need at least one input file to combine.\n");
        return EXIT_FAILURE;
    }

    AGPGraph* graph = init_graph();

    // Read in data
    for (int i = 0; i < argc; i++) {
        fprintf(stderr, "Opening file: %s\n", argv[i]);
        
        FILE* input;
        if ((input  = fopen(argv[i],"r")) == NULL) {
            fprintf(stderr, "Error opening file\n");
            return EXIT_FAILURE;
        }
        
        agp_file* data;
        if (parse_agp_file(input, &data) != 0) {
            fprintf(stderr, "Error parsing fiie\n");
            fclose(input);
            return EXIT_FAILURE;
        }
        fclose(input);

        if (add_to_graph(data, graph)) return EXIT_FAILURE;
    }
    dump_graph(graph);

    return EXIT_SUCCESS;
} 
