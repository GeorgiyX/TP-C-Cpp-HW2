#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "seq_search.h"
#include "utils.h"
#define OUT_FILE_ST "time-st.txt"
#define OUT_FILE_MT "time-mt.txt"


int main(int argc, const char **argv) {
    if (argc < 5) {
        return -1;
    }
    int seq_cnt = atoi(argv[3]);

#ifdef _DYNAMIC
#include <dlfcn.h>
    void *library;
    sequences_vector *(*search_sequences)(const char *, size_t , const char **in_sequences);
    void (*free_sequences_vector)(sequences_vector *vector);
    library = dlopen(argv[1], RTLD_LAZY);
    if (!library) {
        printf("can't load lib in stress test");
        return 1;
    }
    *(void **) (&search_sequences) = dlsym(library, "search_sequences");
    *(void **) (&free_sequences_vector) = dlsym(library, "free_sequences_vector");
    if (!search_sequences || !free_sequences_vector) {
        dlclose(library);
        return -1;
    }
#endif

    double start = omp_get_wtime();
    sequences_vector *result = search_sequences(argv[2], seq_cnt, &argv[4]);

    if (!result) {
#ifdef _DYNAMIC
        dlclose(library);
#endif
        return -1;
    }
    double end = omp_get_wtime();
    double time_in_seconds = end - start;

    free_sequences_vector(result);
    int ret_code = 0;
#ifdef _DYNAMIC
    ret_code = write_to_file(OUT_FILE_MT, time_in_seconds);
    dlclose(library);
#else
    ret_code = write_to_file(OUT_FILE_ST, time_in_seconds);
#endif
    if (ret_code) {
        return -1;
    }
    return 0;
}