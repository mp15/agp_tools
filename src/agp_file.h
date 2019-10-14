// Copyright 2019 Genome Research Limited

#ifndef AGP_FILE_H
#define AGP_FILE_H

#include <stdio.h>
#include <inttypes.h>

typedef enum _comp_type {
    active_finishing = 'A',
    draft_htg = 'D',
    finished_htg = 'F',
    whole_genome_finishing = 'G',
    other_sequence = 'O',
    pre_draft = 'P',
    wgs_contig = 'W',
    known_gap = 'N',
    unknown_gap = 'U',
} comp_type;

typedef struct _agp_line {
    char* object;
    uint64_t object_beg;
    uint64_t object_end;
    int32_t part_number;
    comp_type component_type;
    union {
        char* component_id;
        uint64_t gap_length;
    };
    union {
        uint64_t component_beg;
        char* gap_type;
    };
    union {
        uint64_t component_end;
        char* linkage;
    };
    union {
        char orientation;
        char* linkage_evidence;
    };
} agp_line;

typedef struct _agp_file {
    int major_version;
    int minor_version;
    size_t n_records;
    agp_line** lines;
} agp_file;

// Read in AGP file from file pointer
int parse_agp_file(FILE* input, agp_file** file_out);

#endif // defined(AGP_FILE_H)
