#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>
#include "seq_search.h"
#include "search_mt.h"
#include "search.h"

sequences_vector *search_sequences(const char *file_path, size_t sequences_cnt, const char **in_sequences) {
    if (!file_path || (sequences_cnt && !in_sequences)) {
        return NULL;
    }

    char *data = NULL;
    size_t file_size = 0;
    if (get_mmap_data(file_path, &data, &file_size)) {
        return NULL;
    }

    size_t core_count = get_nprocs() - 0;
    size_t current_seq = 0;
    while (current_seq != sequences_cnt) {
        pthread_t threads[core_count];
        void *threads_return[core_count];
        size_t thread_no = 0;
        for (; (thread_no < core_count) && (current_seq < sequences_cnt); ++thread_no, ++current_seq) {
            thread_data thr_data = {data, in_sequences[current_seq]};
            pthread_create(&(threads[thread_no]), NULL, thread_routine, &thr_data);
        }
        // TODO(G): основной поток стоит.
        for (size_t cur_thread = 0; cur_thread < thread_no; ++cur_thread) {
            pthread_join(threads[cur_thread], &threads_return[cur_thread]);
        }
        long int is_error = 0;
        for (size_t cur_thread = 0; cur_thread < thread_no; ++cur_thread) {
            is_error += (long int) threads_return[cur_thread];
        }
        if (is_error) {
            free_founded_sequence(prev_node_mutex.first_node);
            prev_node_mutex.first_node = NULL;
            break;
        }
    }

    if (munmap(data, file_size)) {
        free_founded_sequence(prev_node_mutex.first_node);
        return NULL;
    }

    sequences_vector *result = NULL;
    result = get_vector_from_list(prev_node_mutex.first_node, in_sequences, sequences_cnt);
    free_founded_sequence(prev_node_mutex.first_node);
    return result;
}
