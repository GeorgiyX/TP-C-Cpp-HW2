#ifndef TP_C_CPP_HW2_SEARCH_MT_H
#define TP_C_CPP_HW2_SEARCH_MT_H

#include "seq_search.h"
#include <pthread.h>
typedef struct thread_data {
    char *data;
    const char *sequence;
} thread_data;

typedef struct node_mutex {
    founded_sequence *prev_node;
    pthread_mutex_t mutex;
} node_mutex;

extern node_mutex prev_node_mutex;


void *thread_routine(void *data);
int get_true_order(const char **sequences, size_t cnt, founded_sequence **root_node);

#endif //TP_C_CPP_HW2_SEARCH_MT_H
