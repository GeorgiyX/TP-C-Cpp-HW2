#include <gtest/gtest.h>
#include <sys/mman.h>
#include <functional>
#include "utils.h"

extern "C" {
#include "search_st.h"
}


TEST(SINGLE_THREAD_TEST, IS_SEQUENCE_IN_DATA_NO) {
    char data[20] = "qwertyui";
    const char *seq = "qriwqeqweqw";
    ASSERT_EQ(is_sequence_in_data(data, seq), 0);
}

TEST(SINGLE_THREAD_TEST, IS_SEQUENCE_IN_DATA_YES) {
    char data[20] = "qwertyui";
    const char *seq = "qri";
    ASSERT_EQ(is_sequence_in_data(data, seq), 1);
}

TEST(SINGLE_THREAD_TEST, GET_MMAP_DATA_YES) {
    size_t file_size = 0;
    char *ptr = nullptr;
    get_mmap_data(testAssist.casesMain[0].dataPath.c_str(), &ptr, &file_size);
    std::function<void(char *)> munmaper = [file_size](char *data) { munmap(data, file_size); };
    std::unique_ptr<char, decltype(munmaper)> data(ptr, munmaper);
    ASSERT_FALSE(!ptr);
}

TEST(SINGLE_THREAD_TEST, GET_MMAP_DATA_NO) {
    size_t file_size = 0;
    char *ptr = nullptr;
    get_mmap_data("not real path", &ptr, &file_size);
    std::function<void(char *)> munmaper = [file_size](char *data) { munmap(data, file_size); };
    std::unique_ptr<char, decltype(munmaper)> data(ptr, munmaper);
    ASSERT_TRUE(!ptr);
}

TEST(SINGLE_THREAD_TEST, GET_ARRAY_FROM_LIST) {
    auto vector = get_vector_from_list(testAssist.casesListToArr[0].list);
    std::unique_ptr<sequences_vector, decltype(&free_sequences_vector)> vector_uniq(vector, &free_sequences_vector);
    if (!vector) { FAIL(); }
    ASSERT_TRUE(*vector == testAssist.casesListToArr[0].answer);
}

TEST(SINGLE_THREAD_TEST, SEARCH_SEQUENCE) {
    for (auto &testCase : testAssist.casesMain) {
        auto vector = search_sequences(testCase.dataPath.c_str(), testCase.task.sequence_count,
                                                    testCase.task.sequence_vector);
        std::unique_ptr<sequences_vector, decltype(&free_sequences_vector)> vector_uniq(vector, &free_sequences_vector);
        if (!vector) { FAIL(); }
        ASSERT_TRUE(*vector == testCase.answer);
    }
}
