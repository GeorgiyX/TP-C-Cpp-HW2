#include <stdlib.h>
#include "search_mt.h"

node_mutex prev_node_mutex = {NULL, NULL,PTHREAD_MUTEX_INITIALIZER};

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
        if (!prev_node_mutex.first_node) {
            prev_node_mutex.first_node = new_node;
        } else {
            prev_node_mutex.prev_node->next = new_node;
        }
        prev_node_mutex.prev_node = new_node;

        errflag = pthread_mutex_unlock(&prev_node_mutex.mutex);
        if (errflag) {
            return (void *) -1;
        }
    }
    return (void *) 0;
}


int get_array_from_list(founded_sequence *list, const char ***array, size_t *cnt, const char **task_array, size_t task_cnt) {
    if (!list || !array || !cnt || !task_array) {
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

    // reorder answer with original sequence order
    size_t new_index = 0;
    for (size_t task_index = 0; task_index < task_cnt; task_index++) {
        const char *sequence = NULL;
        for (size_t answer_index = new_index; answer_index < *cnt; ++answer_index) {
            if (task_array[task_index] == *array[answer_index]) {
                sequence = task_array[task_index];
            }
        }
        if (sequence) {
            *array[new_index++] = sequence;
        }
    }
    return 0;
}
