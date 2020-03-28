#ifndef TP_C_CPP_HW2_SEARCH_H
#define TP_C_CPP_HW2_SEARCH_H
// Common inner API

#include "seq_search.h"

typedef struct founded_sequence founded_sequence;
struct founded_sequence {
    const char *sequence;
    founded_sequence *next;
};


int get_mmap_data(const char *path, char **data);
int is_sequence_in_data(char *data, const char *sequence);
void free_founded_sequence(founded_sequence *node);

#endif //TP_C_CPP_HW2_SEARCH_H
