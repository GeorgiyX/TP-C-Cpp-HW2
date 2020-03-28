#include <stdlib.h>
#include "seq_search.h"
#include "search.h"
#include "search_st.h"

const char **search_sequences(const char *file_path, size_t sequences_cnt, const char **in_sequences) {
    if (!file_path || (sequences_cnt && !in_sequences)) {
        return NULL;
    }

    char *data = NULL;
    if (get_mmap_data(file_path, &data)) {
        return NULL;
    }

    founded_sequence *first_node = NULL;
    founded_sequence *prev_node = first_node;
    for (int task_index = 0; task_index < sequences_cnt; ++task_index) {
        if (is_sequence_in_data(data, in_sequences[task_index])) {
            founded_sequence *new_node = calloc(1, sizeof(founded_sequence));
            if (!new_node) {
                free_founded_sequence(first_node);
                first_node = NULL;
                break;
            }
            if (!first_node) {
                first_node = new_node;
            } else {
                prev_node->next = new_node;
            }
            new_node->sequence = in_sequences[task_index];
            prev_node = new_node;
        }
    }
    free(data);
    const char **result;
    size_t result_cnt = 0;
    if (get_array_from_list(first_node, &result, &result_cnt)) {
        result = NULL;
    }
    free_founded_sequence(first_node);
    return result;
}
