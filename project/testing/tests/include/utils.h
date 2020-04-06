//
// Created by adminu on 24.03.2020.
//

#ifndef TP_C_CPP_HW2_UTILS_H
#define TP_C_CPP_HW2_UTILS_H

#include <gtest/gtest.h>
#include <experimental/filesystem>
#include <dlfcn.h>

extern "C" {
#include "seq_search.h"
#include "search.h"
typedef struct node_mutex node_mutex;
}

namespace fs = std::experimental::filesystem;

class MULTI_THREAD_TEST : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;
    template <class T>
    void getCodePtr(void *&lib, T &fooPtr, const char *fooName) {
        *(void **) (&fooPtr) = dlsym(lib, fooName);
        auto error = dlerror();
        if (error) {
            std::cout << error;
            throw error;
        }
    }

    sequences_vector *(*search_sequences_mt)(const char *, size_t , const char **in_sequences);
    void *(*free_founded_sequence_mt)(founded_sequence *);
    void (*free_sequences_vector_mt)(sequences_vector *vector);
    void *(*thread_routine_mt)(void *);
    void *(*get_mmap_data_mt)(const char *, char **, size_t*);
    int (*is_sequence_in_data_mt)(char *, const char *);
    sequences_vector *(*get_vector_from_list_mt)(founded_sequence *, const char **, size_t );
    node_mutex *prev_node_mutex;

    void *library;
};

struct TestCaseMain {
    std::string dataPath;
    sequences_vector task;
    std::vector<std::string> answer;
};

struct TestCaseListToArr {
    founded_sequence *list;
    sequences_vector vecForOrder;
    std::vector<std::string> answer;
};

bool operator==(sequences_vector &predictAnswer, std::vector<std::string> &trueAnswer);

struct TestAssist {
    TestAssist() = default;
    ~TestAssist();
    std::string libPath;
    std::vector<TestCaseMain> casesMain;
    std::vector<TestCaseListToArr> casesListToArr;
    static founded_sequence *shakeList(founded_sequence *list);
    void setCasesListToArr(fs::path &dir);
    void setCasesMain(fs::path &dir);
    void setTestCases(fs::path dir);
    static sequences_vector getSeqVector(std::string &string);
};
extern TestAssist testAssist;
#endif //TP_C_CPP_HW2_UTILS_H
