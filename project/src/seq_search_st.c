#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "seq_search.h"
#include "search.h"

founded_sequence *search_sequences(const char *file_path, size_t sequences_cnt, const char **in_sequences) {
    if (!file_path || (sequences_cnt && !in_sequences)) {
        return NULL;
    }

    char *data = NULL;
    if (get_mmap_data(file_path, &data)) {
        return NULL;
    }

    founded_sequence *result = calloc(1, sizeof(founded_sequence));
    if (!result) {
        return NULL;
    }

    founded_sequence *prev_node = result;
    for (int cur_sequence = 0; cur_sequence < sequences_cnt; ++cur_sequence) {
        if (is_sequence_in_data(data, in_sequences[cur_sequence])) {
            founded_sequence *new_node = calloc(1 , sizeof(founded_sequence));
            if (!new_node) {
                free_founded_sequence(result);
                result = NULL;
                break;
            }
            new_node->sequence = in_sequences[cur_sequence];
            prev_node->next = new_node;
            prev_node = new_node;
        }
    }
    return result;
}

