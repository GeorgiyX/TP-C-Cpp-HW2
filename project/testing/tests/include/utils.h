//
// Created by adminu on 24.03.2020.
//

#ifndef TP_C_CPP_HW2_UTILS_H
#define TP_C_CPP_HW2_UTILS_H

#include <gtest/gtest.h>
#include <experimental/filesystem>

extern "C" {
#include "seq_search.h"
#include "search.h"
#include "search_mt.h"
}

namespace fs = std::experimental::filesystem;

class MULTI_THREAD_TEST : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    const char **(*search_sequences_mt)(const char *, size_t , const char **);
    void *(*free_founded_sequence_mt)(founded_sequence *);
    void *(*thread_routine_mt)(void *);
    void *(*get_mmap_data_mt)(const char *, char **);
    int (*is_sequence_in_data_mt)(char *, const char *);
    int (*get_true_order_mt)(const char **, size_t, founded_sequence **);

    void *library;
};

struct TestCase {
    std::string dataPath;
    size_t taskCount;
    const char **task;
    std::vector<std::string> answer;
};

struct TestAssist {
    static std::string libPath;
    static std::vector<TestCase> testCases;
    static void setTestCases(fs::path dir);
    static void getArray(std::string &string, const char ***ptr, size_t *cnt);
};

#endif //TP_C_CPP_HW2_UTILS_H
