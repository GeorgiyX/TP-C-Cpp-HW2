#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "seq_search.h"
#include "utils.h"
#define OUT_FILE_ST "time-st.txt"
#define OUT_FILE_MT "time-mt.txt"


int main(int argc, const char **argv) {
    if (argc < 3) {
        return -1;
    }
    int seq_cnt = atoi(argv[2]);

#ifdef _DYNAMIC
#include <dlfcn.h>
    void *library;
    const char **(*search_sequences)(const char *, size_t , const char **);
    library = dlopen("./../../../libsequence_search_dynamic.so", RTLD_LAZY);
    if (!library) {
        printf("can't load lib in stress_tst");
        return 1;
    }
    *(void **) (&search_sequences) = dlsym(library, "search_sequences");
#endif
    time_t start = time(0);
    const char **result = search_sequences(argv[1], seq_cnt, &argv[3]);
    if (!result) {
#ifdef _DYNAMIC
        dlclose(library);
#endif
        return -1;
    }
    time_t end = time(0);
    double time_in_seconds = difftime(end, start);

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