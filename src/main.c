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
    fprintf(stderr, "Opening file: %s\n", argv[0]);
    
    FILE* input;
    if ((input  = fopen(argv[0],"r")) == NULL) {
        fprintf(stderr, "Error opening fiie\n");
        return EXIT_FAILURE;
    }
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, input)) >= 0) {
        if (linelen > 0 && line[0] == '#') {
            continue;
        }
        char object[50];
        int object_beg;
        int object_end;
        int part_number;
        char component_type;
        char component_id_gap_length[50];
        char component_beg_gap_type[50];
        char component_end_linkage[50];
        char orientation_evidence[50];
        sscanf(line, "%50s\t%d\t%d\t%d\t%c\t%50s\t%50s\t%50s\t%50s",
           object,
           &object_beg,
           &object_end,
           &part_number,
           &component_type,
           component_id_gap_length,
           component_beg_gap_type,
           component_end_linkage,
           orientation_evidence
        );
        
        AGPNode* node = (AGPNode*)malloc(sizeof(AGPNode));
        printf("%s\n", object);
    }

    fclose(input);

    return EXIT_SUCCESS;
} 
