#include <gtest/gtest.h>
#include "utils.h"

extern "C" {
#include "sequence_search.h"
}


TEST(BASE_TESTS, STATIC_LIB_TST) {
    ASSERT_EQ(hello(), 1);
}

TEST_F(SequenceSearchDynamic, DYNAMIC_LIB_TST) {
    ASSERT_EQ(hello_so(), 1);
}
