#include <stdlib.h>
#include "stdio.h"
#include "search_mt.h"

node_mutex prev_node_mutex = {NULL, NULL, PTHREAD_MUTEX_INITIALIZER};

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
            free(new_node);
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


sequences_vector *get_vector_from_list(founded_sequence *list, const char **task_array, size_t task_cnt) {
    if (!list || !task_array) {
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

    // reorder answer with original sequence order
    size_t new_index = 0;
    for (size_t task_index = 0; task_index < task_cnt; task_index++) {
        founded_sequence *node = list;
        while (node) {
            if (task_array[task_index] == node->sequence) { break; }
            node = node->next;
        }

        if (node) {
            vector->sequence_vector[new_index++] = node->sequence;
        }
    }
    return vector;
}
