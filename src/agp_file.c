#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "agp_file.h"

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

int parse_agp_file(FILE* input)
{
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    agp_line* data = NULL;
    while ((linelen = getline(&line, &linecap, input)) >= 0) {
        // skip empty or comment lines
        if (linelen > 0 && line[0] == '#') {
            continue;
        }
        if (parse_agp_line(line, &data) != 0 ) {
            return 1;
        }
        free(data);
    }
    return 0;
}
