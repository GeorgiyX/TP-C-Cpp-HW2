#include <gtest/gtest.h>
#include "utils.h"

TestAssist testAssist;

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    if (argc < 3) {
        return -1;
    }
    testAssist.setTestCases(fs::path(argv[1]));
    std::cout << "Path to data dir: " << argv[1] << std::endl;
    testAssist.libPath = argv[2];
    return RUN_ALL_TESTS();
}
