// Copyright 2019 Genome Research Limited

#ifndef AGP_FILE_H
#define AGP_FILE_H

typedef struct _AGPEntry AGPEntry;

struct _AGPEntry {
    const char* id;
    unsigned int begin;
    unsigned int end;
};


#endif // defined(AGP_FILE_H)
