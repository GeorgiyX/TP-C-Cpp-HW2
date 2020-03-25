#include "utils.h"
#include <dlfcn.h>
#include <fstream>


void MULTI_THREAD_TEST::SetUp() {
    Test::SetUp();

    library = dlopen("./../../../libsequence_search_dynamic.so", RTLD_LAZY);
    if (!library) { FAIL(); }
    *(void **) (&search_sequences_mt) = dlsym(library, "search_sequences");
    *(void **) (&free_founded_sequence_mt) = dlsym(library, "free_founded_sequence");
    *(void **) (&is_sequence_in_data_mt) = dlsym(library, "is_sequence_in_data");
    *(void **) (&thread_routine_mt) = dlsym(library, "thread_routine");
    *(void **) (&get_mmap_data_mt) = dlsym(library, "get_mmap_data");
    *(void **) (&get_true_order_mt) = dlsym(library, "get_true_order");
}

void MULTI_THREAD_TEST::TearDown() {
    Test::TearDown();
    dlclose(library);
}

void TestAssist::setTestCases(fs::path dir) {
    for (auto &fileName : fs::directory_iterator(dir / "TASKS")) {
        std::ifstream file(fileName.path());
        if (!file)
            throw "can't open file";
        TestCase testCase = {};
        std::getline(file, testCase.dataPath);

        std::string task;
        std::getline(file, task);
        getArray(task, &testCase.task, &testCase.taskCount);

        std::string answer;
        std::getline(file, answer);
        std::istringstream iss(answer);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(testCase.answer));

        TestAssist::testCases.push_back(testCase);
    }
}

void TestAssist::getArray(std::string &string, const char ***ptr, size_t *cnt) {
    if (!ptr || !cnt)
        throw "wrong arg";
    std::istringstream iss(string);

    std::vector<std::string> sequencesVec;
    std::vector<const char *> cstrVec;
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(sequencesVec));
    for (std::string &sequence : sequencesVec) {
        (*cnt)++;
        cstrVec.emplace_back(sequence.c_str());
    }
    *ptr = cstrVec.data();
}
