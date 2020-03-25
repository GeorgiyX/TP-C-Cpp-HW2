#include <gtest/gtest.h>
#include "utils.h"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    if (argc < 2) {
        return -1;
    }
    std::cout << "Path to data dir: " << argv[1] << std::endl;
    TestAssist::setTestCases(fs::path(argv[1]));
    return RUN_ALL_TESTS();
}
