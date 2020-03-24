//
// Created by adminu on 24.03.2020.
//

#ifndef TP_C_CPP_HW2_UTILS_H
#define TP_C_CPP_HW2_UTILS_H

#include <gtest/gtest.h>


class SequenceSearchDynamic : public ::testing::Test {
protected:
    void SetUp() override;

    void TearDown() override;
    int val;
    int (*hello_so)(void);
    void *library;
};

#endif //TP_C_CPP_HW2_UTILS_H
