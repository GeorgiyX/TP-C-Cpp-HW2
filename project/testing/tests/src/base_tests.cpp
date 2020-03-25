#include <gtest/gtest.h>
#include "utils.h"

extern "C" {
#include "seq_search.h"
#include "search.h"
}


TEST(SINGLE_THREAD_TEST, IS_SEQUENCE_IN_DATA_0) {
    char data[20] = "qwertyui";
    const char *seq = "qriwqeqweqw";
    ASSERT_EQ(is_sequence_in_data(data, seq), 0);
}

TEST(SINGLE_THREAD_TEST, IS_SEQUENCE_IN_DATA_1) {
    char data[20] = "qwertyui";
    const char *seq = "qri";
    ASSERT_EQ(is_sequence_in_data(data, seq), 1);
}

TEST_F(MULTI_THREAD_TEST, IS_SEQUENCE_IN_DATA_0) {
    char data[20] = "qwertyui";
    const char *seq = "qriwqeqweqw";
    ASSERT_EQ(is_sequence_in_data_mt(data, seq), 0);
}

TEST_F(MULTI_THREAD_TEST, IS_SEQUENCE_IN_DATA_1) {
    char data[20] = "qwertyui";
    const char *seq = "qri";
    ASSERT_EQ(is_sequence_in_data_mt(data, seq), 1);
}

TEST(SINGLE_THREAD_TEST, is_sequence_in_data_1) {

    is_sequence_in_data()

}