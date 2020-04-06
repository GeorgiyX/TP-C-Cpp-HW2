#include <stdlib.h>
#include "search_st.h"

sequences_vector *get_vector_from_list(founded_sequence *list) {
    if (!list) {
        return NULL;
    }

    sequences_vector *vector = NULL;
    if (!(vector = calloc(1, sizeof(sequences_vector)))) {
        return NULL;
    }

    founded_sequence *next = list;
    while (next) {
        vector->sequence_count++;
        next = next->next;
    }

    if (!(vector->sequence_vector = calloc(vector->sequence_count, sizeof(const char *)))) {
        free(vector);
        return NULL;
    }

    next = list;
    size_t array_index = 0;
    while (next) {
        vector->sequence_vector[array_index++] = next->sequence;
        next = next->next;
    }
    return vector;
}
