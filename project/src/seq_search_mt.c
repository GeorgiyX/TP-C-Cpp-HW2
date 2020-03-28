#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "seq_search.h"
#include "search_mt.h"
#include "search.h"

const char **search_sequences(const char *file_path, size_t sequences_cnt, const char **in_sequences) {
    if (!file_path || (sequences_cnt && !in_sequences)) {
        return NULL;
    }

    char *data = NULL;
    if (get_mmap_data(file_path, &data)) {
        return NULL;
    }

    int core_count = get_nprocs() - 1;
    size_t current_seq = 0;
    while (current_seq < sequences_cnt) {
        pthread_t threads[core_count];
        void *threads_return[core_count];
        for (int cur_thread = 0; cur_thread < core_count; ++cur_thread) {
            thread_data thr_data = {data, in_sequences[current_seq++]};
            pthread_create(&(threads[cur_thread]), NULL, thread_routine, &thr_data);
        }
        // TODO(G): основной поток стоит.
        for (int cur_thread = 0; cur_thread < core_count; ++cur_thread) {
            pthread_join(threads[cur_thread], &threads_return[cur_thread]);
        }
        int is_error = 0;
        for (int cur_thread = 0; cur_thread < core_count; ++cur_thread) {
            is_error += *((int *) threads_return[cur_thread]);
        }
        if (is_error) {
            free_founded_sequence(prev_node_mutex.first_node);
            prev_node_mutex.first_node = NULL;
            break;
        }
    }

    free(data);
    const char **result;
    size_t result_cnt = 0;
    if (get_array_from_list(prev_node_mutex.first_node, &result, &result_cnt, in_sequences, sequences_cnt)) {
        result = NULL;
    }
    free_founded_sequence(prev_node_mutex.first_node);
    return result;
}
