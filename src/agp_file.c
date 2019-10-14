#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "agp_file.h"

// Parse a single non-comment line from an AGP file
static int parse_agp_line(const char* restrict line, agp_line** parsed)
{
    agp_line* data = (agp_line*)malloc(sizeof(agp_line));
    char* parsing_line = strdup(line);
    char* basepointer = parsing_line;
    char* token;

    if ((token = strsep(&parsing_line, "\t")) != NULL) {
        data->object = strdup(token);
    } else { goto badline; }
        
    if ((token = strsep(&parsing_line, "\t")) != NULL) {
        data->object_beg = strtoull(token, NULL, 0);
        if (errno != 0 ) { goto badline; }
    } else { goto badline; }

    if ((token = strsep(&parsing_line, "\t")) != NULL) {
        data->object_end = strtoull(token, NULL, 0);
        if (errno != 0 ) { goto badline; }
    } else { goto badline; }

    if ((token = strsep(&parsing_line, "\t")) != NULL) {
        data->part_number = strtoul(token, NULL, 0);
        if (errno != 0 ) { goto badline; }
    } else { goto badline; }

    if ((token = strsep(&parsing_line, "\t")) != NULL) {
        data->component_type = (comp_type) token[0];
    } else { goto badline; }

    // If component type is U or N then next 4 fields have different meaning
    if (data->component_type == 'U' || data->component_type == 'N') {
        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->gap_length = strtoull(token, NULL, 0);
            if (errno != 0 ) { goto badline; }
        } else { goto badline; }
            
        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->gap_type = strdup(token);
        } else { goto badline; }

        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->linkage = strdup(token);
        } else { goto badline; }

        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->linkage_evidence = strdup(token);
        } else { goto badline; }
    } else {
        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->component_id = strdup(token);
        } else { goto badline; }
        
        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->component_beg = strtoull(token, NULL, 0);
            if (errno != 0 ) { goto badline; }
        } else { goto badline; }
        
        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->component_end = strtoull(token, NULL, 0);
            if (errno != 0 ) { goto badline; }
        } else { goto badline; }
        
        if ((token = strsep(&parsing_line, "\t")) != NULL) {
            data->orientation = token[0];
        } else { goto badline; }
    }

    free(basepointer);

    *parsed = data;
    return 0;

badline:
    free(data); free(basepointer);
    return 1;
}

typedef struct _agp_line_ll agp_line_ll;
struct _agp_line_ll {
    agp_line* data;
    agp_line_ll *next;
};

int parse_agp_file(FILE* input, agp_file** file_out)
{
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    agp_line* data = NULL;
    agp_line_ll* base = NULL;
    agp_file* file = (agp_file*)malloc(sizeof(agp_file));
    file->n_records = 0;

    while ((linelen = getline(&line, &linecap, input)) >= 0) {
        // skip empty or comment lines
        if (linelen > 0 && line[0] == '#') {
            continue;
        }
        if (parse_agp_line(line, &data) != 0 ) {
            return 1;
        }

        agp_line_ll* add = (agp_line_ll*)malloc(sizeof(agp_line_ll));
        add->data = data;
        add->next = base;
        base = add;

        file->n_records++;
    }

    if (file->n_records > 0) {
        file->lines = (agp_line**)malloc(sizeof(agp_line*)*file->n_records);
        size_t i = file->n_records;
        do {
            i--;
            file->lines[i] = base->data;
            agp_line_ll* del = base;
            base = base->next;
            free(del);
        } while (base);
    } else {file->lines = NULL;}
    *file_out = file;
    
    return 0;
}
