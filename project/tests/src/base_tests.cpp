#include <gtest/gtest.h>
#include <dlfcn.h>

extern "C" {
#include "sequence_search.h"
}


TEST(BASE_TESTS, STATIC_LIB_TST) {
    ASSERT_EQ(hello(), 1);
}

TEST(BASE_TESTS, DYNAMIC_LIB_TST) {
    void *library;
    int (*hello_so)(void);
    library = dlopen("./../../libsequence_search_dynamic.so", RTLD_LAZY);
    if (!library) { FAIL(); }
    *(void **) (&hello_so) = dlsym(library, "hello");  // or reinterpret_cast
    ASSERT_EQ(hello_so(), 1);
    dlclose(library);
}
