#include "utils.h"
#include <dlfcn.h>


void SequenceSearchDynamic::SetUp() {
    Test::SetUp();
    std::cout << "SetUp";
    val = 100;

    library = dlopen("./../../libsequence_search_dynamic.so", RTLD_LAZY);
    if (!library) { FAIL(); }
//    *(void **) (&hello_so) = dlsym(library, "hello");  // or reinterpret_cast

}

void SequenceSearchDynamic::TearDown() {
    Test::TearDown();
    dlclose(library);
}
