#include "utils.h"
#include <dlfcn.h>
#include <fstream>
#include <utils.h>
#include <cstring>


void MULTI_THREAD_TEST::SetUp() {
    Test::SetUp();

    library = dlopen(TestAssist::libPath.c_str(), RTLD_LAZY);
    if (!library) { FAIL(); }
    *(void **) (&search_sequences_mt) = dlsym(library, "search_sequences");
    *(void **) (&free_founded_sequence_mt) = dlsym(library, "free_founded_sequence");
    *(void **) (&is_sequence_in_data_mt) = dlsym(library, "is_sequence_in_data");
    *(void **) (&thread_routine_mt) = dlsym(library, "thread_routine");
    *(void **) (&get_mmap_data_mt) = dlsym(library, "get_mmap_data");
    *(void **) (&get_vector_from_list_mt) = dlsym(library, "get_vector_from_list");
    *(void **) (&prev_node_mutex) = dlsym(library, "prev_node_mutex");
}

void MULTI_THREAD_TEST::TearDown() {
    Test::TearDown();
    dlclose(library);
}

void TestAssist::setTestCases(std::experimental::filesystem::path dir) {
    TestAssist::setCasesMain(dir);
    TestAssist::setCasesListToArr(dir);
}

sequences_vector TestAssist::getSeqVector(std::string &string) {
    sequences_vector vector = {};
    std::istringstream iss(string);
    std::vector<std::string> strVec;
    std::vector<const char *> cStrVec;
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
              std::back_inserter(strVec));
    for (std::string &sequence : strVec) {
        vector.sequence_count++;
        auto cStr = new char[sequence.length() + 1];
        std::memcpy(cStr, sequence.c_str(), sequence.length() + 1);
        cStrVec.push_back(cStr);
    }
    vector.sequence_vector = new const char*[cStrVec.size()];
    std::copy(cStrVec.begin(), cStrVec.end(), vector.sequence_vector);
    return vector;
}

void TestAssist::setCasesListToArr(fs::path dir) {
    for (auto &fileName : fs::directory_iterator(dir / "TASK_LIST_TO_ARR")) {
        std::cout << "add file: " << fileName.path() << std::endl;
        std::ifstream file(fileName.path());
        if (!file)
            throw "can't open file";
        TestCaseListToArr testCase = {};

        std::string sequence;
        std::getline(file, sequence);

        testCase.vecForOrder = getSeqVector(sequence);

        auto prevNode = testCase.list;
        for (size_t sequenceIndex = 0; sequenceIndex < testCase.vecForOrder.sequence_count; ++sequenceIndex) {
            auto newNode = new founded_sequence {};
            newNode->sequence = testCase.vecForOrder.sequence_vector[sequenceIndex];
            std::cout << "newNode->sequence: " << newNode->sequence << " index: " << sequenceIndex << std::endl;
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

        TestAssist::casesListToArr.push_back(testCase);
    }
}

void TestAssist::setCasesMain(fs::path dir) {
    for (auto &fileName : fs::directory_iterator(dir / "TASKS")) {
        std::cout << "add file: " << fileName.path() << std::endl;
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

        TestAssist::casesMain.push_back(testCase);
    }
}

founded_sequence * TestAssist::shakeList(founded_sequence *list) {
    auto current = list;
    auto last = list;
    founded_sequence * prev = nullptr;
    while (current) {
        auto next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    last->next = nullptr;
    return prev;
}

std::string TestAssist::libPath;
std::vector<TestCaseMain> TestAssist::casesMain;
std::vector<TestCaseListToArr> TestAssist::casesListToArr;

bool operator==(sequences_vector &predictAnswer, std::vector<std::string> &trueAnswer) {
    if (predictAnswer.sequence_count != trueAnswer.size()) {
        std::cout << "!= cnt:: l: " << predictAnswer.sequence_count << " r: " << trueAnswer.size() << std::endl;
        return false;
    }
    bool result = true;
    for (size_t answerIndex = 0; answerIndex < trueAnswer.size(); ++answerIndex) {
        if (std::string(predictAnswer.sequence_vector[answerIndex]) != trueAnswer[answerIndex]) {
            std::cout << "l: " << std::string(predictAnswer.sequence_vector[answerIndex]) << " r: " << trueAnswer[answerIndex] << std::endl;
            result = false;
            break;
        }
    }
    return result;
}
