#include <stdlib.h>
#include "search.h"
#include "search_mt.h"

node_mutex prev_node_mutex = {NULL, PTHREAD_MUTEX_INITIALIZER};

void *thread_routine(void *data) {
    thread_data *args = (thread_data *) data;
    if (is_sequence_in_data(args->data, args->sequence)) {
        founded_sequence *new_node = calloc(1, sizeof(founded_sequence));
        if (!new_node) {
            return (void *) -1;
        }

        new_node->sequence = args->sequence;
        int errflag = pthread_mutex_lock(&prev_node_mutex.mutex);
        if (errflag) {
            return (void *) -1;
        }

        prev_node_mutex.prev_node->next = new_node;
        prev_node_mutex.prev_node = new_node;
        errflag = pthread_mutex_unlock(&prev_node_mutex.mutex);
        if (errflag) {
            return (void *) -1;
        }

    }
    return (void *) 0;
}

int get_true_order(const char **sequences, size_t cnt, founded_sequence **root_node) {
    if (!root_node || !*root_node) {
        return -1;
    }

    founded_sequence *tmp_node = *root_node;
    size_t arr_size = 0;
    while (tmp_node) {
        arr_size++;
        tmp_node = tmp_node->next;
    }
    founded_sequence **seq_arr = calloc(arr_size, sizeof(founded_sequence *));
    if (!seq_arr) {
        return -1;
    }

    tmp_node = *root_node;
    size_t cur_elem = 0;
    while (tmp_node++) {
        cur_elem++;
        seq_arr[cur_elem] = tmp_node;
    }

    founded_sequence *prev_node = NULL;
    for (int cur_seq = 0; cur_seq < cnt; ++cur_seq) {
        founded_sequence *founded_node = NULL;
        for (int cur_node = 0; cur_node < arr_size; ++cur_node) {
            if (seq_arr[cur_node]->sequence == sequences[cur_seq]) {
                founded_node = seq_arr[cur_node];
            }
        }
        if (founded_node && !prev_node) {
            prev_node = founded_node;
            *root_node = prev_node;
        } else if (founded_node) {
            prev_node->next = founded_node;
            prev_node = founded_node;
        }
    }
    return 0;
}
