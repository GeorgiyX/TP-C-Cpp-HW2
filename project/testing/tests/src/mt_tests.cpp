#include <gtest/gtest.h>
#include <sys/mman.h>
#include <functional>
#include "utils.h"

extern "C" {
#include "search_mt.h"
}

node_mutex prev_node_mutex = {nullptr, nullptr,PTHREAD_MUTEX_INITIALIZER};

TEST_F(MULTI_THREAD_TEST, IS_SEQUENCE_IN_DATA_NO) {
    char data[20] = "qwertyui";
    const char *seq = "qriwqeqweqw";
    ASSERT_EQ(is_sequence_in_data_mt(data, seq), 0);
}

TEST_F(MULTI_THREAD_TEST, IS_SEQUENCE_IN_DATA_YES) {
    char data[20] = "qwertyui";
    const char *seq = "qri";
    ASSERT_EQ(is_sequence_in_data_mt(data, seq), 1);
}

TEST_F(MULTI_THREAD_TEST, THREAD_ROUTINE) {
    pthread_t thread;
    void *thread_return;
    char data[20] = "qwertyui";
    const char *seq = "qri";
    thread_data thr_data = {data, seq};
    pthread_create(&thread, nullptr, thread_routine_mt, &thr_data);
    pthread_join(thread, &thread_return);

    if (reinterpret_cast<long long int>(thread_return)) {
        SUCCEED();
        std::free(prev_node_mutex->first_node);
        return;
    }

    if (!prev_node_mutex->first_node) {
        FAIL();
    }
    auto result = prev_node_mutex->first_node->sequence;
    std::free(prev_node_mutex->first_node);
    prev_node_mutex->first_node = nullptr;
    ASSERT_EQ(result, seq);
}

TEST_F(MULTI_THREAD_TEST, GET_MMAP_DATA_YES) {
    size_t file_size = 0;
    char *ptr = nullptr;
    get_mmap_data_mt(TestAssist::casesMain[0].dataPath.c_str(), &ptr, &file_size);
    std::function<void(char *)> munmaper = [file_size](char *data) { munmap(data, file_size); };
    std::unique_ptr<char, decltype(munmaper)> data(ptr, munmaper);
    ASSERT_FALSE(!ptr);
}

TEST_F(MULTI_THREAD_TEST, GET_MMAP_DATA_NO) {
    size_t file_size = 0;
    char *ptr = nullptr;
    get_mmap_data_mt("not real path", &ptr, &file_size);
    std::function<void(char *)> munmaper = [file_size](char *data) { munmap(data, file_size); };
    std::unique_ptr<char, decltype(munmaper)> data(ptr, munmaper);
    ASSERT_TRUE(!ptr);
}

TEST_F(MULTI_THREAD_TEST, GET_ARRAY_FROM_LIST) {
    TestAssist::casesListToArr[0].list = TestAssist::shakeList(TestAssist::casesListToArr[0].list);
    auto vector = get_vector_from_list_mt(TestAssist::casesListToArr[0].list,
                                          TestAssist::casesListToArr[0].vecForOrder.sequence_vector,
                                          TestAssist::casesListToArr[0].vecForOrder.sequence_count);
    TestAssist::casesListToArr[0].list = TestAssist::shakeList(TestAssist::casesListToArr[0].list);
    std::unique_ptr<sequences_vector, decltype(&std::free)> vector_uniq(vector, &std::free);
    if (!vector) { FAIL(); }
    ASSERT_TRUE(*vector == TestAssist::casesListToArr[0].answer);
}

TEST_F(MULTI_THREAD_TEST, SEARCH_SEQUENCE) {
    for (auto &testCase : TestAssist::casesMain) {
        auto vector = search_sequences_mt(testCase.dataPath.c_str(), testCase.task.sequence_count,
                                       testCase.task.sequence_vector);
        std::unique_ptr<sequences_vector, decltype(&free_sequences_vector)> vector_uniq(vector, &free_sequences_vector);
        if (!vector) { FAIL(); }

        std::cout << "SEARCH_SEQUENCE: " << testCase.dataPath << "\n\tl size:" << vector->sequence_count << "\n\tr size:" << testCase.answer.size() << std::endl;

        std::cout << "TASK content:" << std::endl;
        for (size_t i = 0; i < testCase.task.sequence_count; ++i) {
            std::cout << "\t" << i << ": " << testCase.task.sequence_vector[i] << std::endl;
        }

        std::cout << "l content:" << std::endl;
        for (size_t i = 0; i < vector->sequence_count; ++i) {
            std::cout << "\t" << i << ": " << vector->sequence_vector[i] << std::endl;
        }

        std::cout << "r content:" << std::endl;
        for (size_t i = 0; i < testCase.answer.size(); ++i) {
            std::cout << "\t" << i << ": " << testCase.answer[i] << std::endl;
        }

        std::cout << "=========END OF TASK=========\n";
        ASSERT_TRUE(*vector == testCase.answer);
    }
}
