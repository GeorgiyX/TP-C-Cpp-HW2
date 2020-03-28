#include <gtest/gtest.h>
#include "utils.h"

extern "C" {
#include "seq_search.h"
#include "search.h"
}


TEST(BASE_TEST, IS_SEQUENCE_IN_DATA_0) {
    char data[20] = "qwertyui";
    const char *seq = "qriwqeqweqw";
    ASSERT_EQ(is_sequence_in_data(data, seq), 0);
}

TEST(BASE_TEST, IS_SEQUENCE_IN_DATA_1) {
    char data[20] = "qwertyui";
    const char *seq = "qri";
    ASSERT_EQ(is_sequence_in_data(data, seq), 1);
}

TEST(BASE_TEST, GET_MMAP_DATA) {
    char *data = nullptr;
    get_mmap_data(TestAssist::testCases[0].dataPath.c_str(), &data);
    ASSERT_EQ(!data, false);
    free(data);
}
