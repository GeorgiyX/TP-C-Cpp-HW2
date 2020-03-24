#include <stdio.h>
#include "sequence_search.h"


int main() {
#ifdef _DYNAMIC
#include <dlfcn.h>
    void *library;
    int (*hello)(void);
    library = dlopen("./../../libsequence_search_dynamic.so", RTLD_LAZY); //path
    if (!library) {
        printf("can't load lib in str_tst");
        dlclose(library);
        return 1;
    }
    *(void **) (&hello) = dlsym(library, "hello");
#endif
    hello();
#ifdef _DYNAMIC
    dlclose(library);
#endif
    return 0;
}