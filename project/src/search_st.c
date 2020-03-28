#include <stdlib.h>
#include "search_st.h"

int get_array_from_list(founded_sequence *list, const char ***array, size_t *cnt) {
    if (!list || !array || !cnt) {
        return -1;
    }
    founded_sequence *next = list;
    *cnt = 0;
    while (next) {
        (*cnt)++;
        next = next->next;
    }
    *array = NULL;
    *array = calloc(*cnt, sizeof(const char *));
    if (!*array) {
        return -1;
    }
    next = list;
    size_t array_index = 0;
    while (next) {
        *array[array_index] = next->sequence;
        next = next->next;
    }
    return 0;
}