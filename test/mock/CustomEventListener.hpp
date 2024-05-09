#pragma once
#include "gtest/gtest.h"
#include "SomeipWrapper.hpp"


class CustomEventListener : public ::testing::EmptyTestEventListener {

    inline static SomeipWrapper someIpWrapperInstance_;
    void OnTestProgramStart(const ::testing::UnitTest& /*unit_test*/) override {
        someIpWrapperInstance_.init();
        someIpWrapperInstance_.start();
    }

    // Called after all test activity ends.
    void OnTestProgramEnd(const ::testing::UnitTest& /*unit_test*/) override {
        someIpWrapperInstance_.stop();
    }
public:
    static SomeipWrapper& getSomeipWrapperInstance() {
        return someIpWrapperInstance_;
    }
};