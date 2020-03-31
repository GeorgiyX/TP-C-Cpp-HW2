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

    prev_node_mutex.first_node = NULL;
    size_t core_count = get_nprocs() - 1;
    size_t current_seq = 0;
    while (current_seq != sequences_cnt) {
        size_t thread_cnt = ((sequences_cnt - current_seq) > core_count) ? core_count : sequences_cnt - current_seq;
        printf("thread_cnt: [%zu] core_count: [%zu] \n", thread_cnt, core_count);
        pthread_t threads[thread_cnt];
        thread_data thr_data[thread_cnt];
        void *threads_return[thread_cnt];
        for (size_t cur_thread = 0; cur_thread < thread_cnt; ++cur_thread, ++current_seq) {
            thr_data[cur_thread].data = data;
            thr_data[cur_thread].sequence =in_sequences[current_seq];
            printf("Create thread with: [%s] and current_seq: [%zu] and cur_thread: [%zu] and thread id: [%ul]\n",thr_data[cur_thread].sequence, current_seq, cur_thread, &(threads[cur_thread]));

            pthread_create(&(threads[cur_thread]), NULL, thread_routine, &thr_data[cur_thread]);
        }
        // TODO(G): основной поток стоит.
        for (size_t cur_thread = 0; cur_thread < thread_cnt; ++cur_thread) {
            pthread_join(threads[cur_thread], &threads_return[cur_thread]);
        }
        long int is_error = 0;
        for (size_t cur_thread = 0; cur_thread < thread_cnt; ++cur_thread) {
            is_error += (long int) threads_return[cur_thread];
        }
        if (is_error) {
            printf("ALARMA! <ERROR>\n");
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
