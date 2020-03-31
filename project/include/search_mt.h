#ifndef TP_C_CPP_HW2_SEARCH_MT_H
#define TP_C_CPP_HW2_SEARCH_MT_H
// Multi Thread inner API

#include <pthread.h>
#include "search.h"

typedef struct thread_data {
    char *data;
    const char *sequence;
} thread_data;

typedef struct node_mutex {
    founded_sequence *first_node;
    founded_sequence *prev_node;
    pthread_mutex_t mutex;
} node_mutex;

extern node_mutex prev_node_mutex;

void *thread_routine(void *data);
sequences_vector *get_vector_from_list(founded_sequence *list, const char **task_array, size_t task_cnt);

#endif //TP_C_CPP_HW2_SEARCH_MT_H
