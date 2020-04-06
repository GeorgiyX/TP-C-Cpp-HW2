#include <dlfcn.h>
#include <fstream>
#include <cstring>
#include "utils.h"


void MULTI_THREAD_TEST::SetUp() {
    Test::SetUp();

    library = dlopen(testAssist.libPath.c_str(), RTLD_LAZY);
    if (!library) { FAIL(); }
    getCodePtr(library, search_sequences_mt, "search_sequences");
    getCodePtr(library, free_founded_sequence_mt, "free_founded_sequence");
    getCodePtr(library, is_sequence_in_data_mt, "is_sequence_in_data");
    getCodePtr(library, thread_routine_mt, "thread_routine");
    getCodePtr(library, get_mmap_data_mt, "get_mmap_data");
    getCodePtr(library, get_vector_from_list_mt, "get_vector_from_list");
    getCodePtr(library, prev_node_mutex, "prev_node_mutex");
    getCodePtr(library, free_sequences_vector_mt, "free_sequences_vector");
}

void MULTI_THREAD_TEST::TearDown() {
    Test::TearDown();
    dlclose(library);
}

void TestAssist::setTestCases(std::experimental::filesystem::path dir) {
    setCasesMain(dir);
    setCasesListToArr(dir);
}

sequences_vector TestAssist::getSeqVector(std::string &string) {
    sequences_vector vector = {};
    std::istringstream iss(string);
    std::vector<std::string> strVec;
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
              std::back_inserter(strVec));
    vector.sequence_vector = new const char *[strVec.size()];
    vector.sequence_count = strVec.size();
    for (size_t strIndex = 0; strIndex < strVec.size(); strIndex++) {
        auto cStr = new char[strVec[strIndex].length() + 1];
        std::memcpy(cStr, strVec[strIndex].c_str(), strVec[strIndex].length() + 1);
        vector.sequence_vector[strIndex] = cStr;
    }
    return vector;
}

void TestAssist::setCasesListToArr(fs::path &dir) {
    for (auto &fileName : fs::directory_iterator(dir / "TASK_LIST_TO_ARR")) {
        std::ifstream file(fileName.path());
        if (!file)
            throw "can't open file";
        TestCaseListToArr testCase = {};

        std::string sequence;
        std::getline(file, sequence);

        testCase.vecForOrder = getSeqVector(sequence);

        auto prevNode = testCase.list;
        for (size_t sequenceIndex = 0; sequenceIndex < testCase.vecForOrder.sequence_count; ++sequenceIndex) {
            auto newNode = new founded_sequence{};
            newNode->sequence = testCase.vecForOrder.sequence_vector[sequenceIndex];
            if (!testCase.list) {
                testCase.list = newNode;
            } else {
                prevNode->next = newNode;
            }
            prevNode = newNode;
        }

        std::istringstream iss(sequence);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                  std::back_inserter(testCase.answer));

        this->casesListToArr.emplace_back(testCase);
    }
}

void TestAssist::setCasesMain(fs::path &dir) {
    for (auto &fileName : fs::directory_iterator(dir / "TASKS")) {
        std::ifstream file(fileName.path());
        if (!file)
            throw "can't open file";
        TestCaseMain testCase = {};
        std::getline(file, testCase.dataPath);
        testCase.dataPath = dir.string() + "BIG_DATA/" + testCase.dataPath;

        std::string task;
        std::getline(file, task);
        testCase.task = getSeqVector(task);

        std::string answer;
        std::getline(file, answer);
        std::istringstream iss(answer);
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                  std::back_inserter(testCase.answer));

        this->casesMain.emplace_back(testCase);
    }
}

founded_sequence *TestAssist::shakeList(founded_sequence *list) {
    auto current = list;
    auto last = list;
    founded_sequence *prev = nullptr;
    while (current) {
        auto next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    last->next = nullptr;
    return prev;
}

TestAssist::~TestAssist() {
    for (auto &testCase : TestAssist::casesMain) {
        for (size_t sequenceIndex = 0; sequenceIndex < testCase.task.sequence_count; ++sequenceIndex) {
            delete[] testCase.task.sequence_vector[sequenceIndex];
        }
        delete[] testCase.task.sequence_vector;
    }
    for (auto &testCase : TestAssist::casesListToArr) {
        auto nodeToFree = testCase.list;
        while (nodeToFree) {
            auto nextNode = nodeToFree->next;
            delete[] nodeToFree->sequence;
            delete nodeToFree;
            nodeToFree = nextNode;
        }
        delete[] testCase.vecForOrder.sequence_vector;
    }
}

bool operator==(sequences_vector &predictAnswer, std::vector<std::string> &trueAnswer) {
    if (predictAnswer.sequence_count != trueAnswer.size()) {
        return false;
    }
    bool result = true;
    for (size_t answerIndex = 0; answerIndex < trueAnswer.size(); ++answerIndex) {
        if (std::string(predictAnswer.sequence_vector[answerIndex]) != trueAnswer[answerIndex]) {
            std::cout << "l: " << std::string(predictAnswer.sequence_vector[answerIndex]) << " r: "
                      << trueAnswer[answerIndex] << std::endl;
            result = false;
            break;
        }
    }
    return result;
}
