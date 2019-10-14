// Copyright 2019 Genome Research Limited

#ifndef AGP_FILE_H
#define AGP_FILE_H

#include <stdio.h>

typedef struct _AGPEntry AGPEntry;

struct _AGPEntry {
    const char* id;
    unsigned int begin;
    unsigned int end;
};

int parse_agp_file(FILE* input);

#endif // defined(AGP_FILE_H)
