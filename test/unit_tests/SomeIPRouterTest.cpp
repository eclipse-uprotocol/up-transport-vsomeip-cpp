#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>
#include "mock/MockSomeIPRouter.hpp"
#include "Logger.h"
#include "mock/MockUCore.hpp"

/**
 *    @brief Test fixture for SomeIPRouter.
 */
class SomeIPRouterTests : public testing::Test {
protected:
    /**
     *    @brief Create SomeIPRouter object for testing.
     */
    MockSomeIPRouter* mockSomeIPR_ = nullptr;
    /**
     *    @brief UCore object used for initialization other other objects.
     */
    MockUCore mockUCore_;
    /**
     *    @brief Holds test log info. Will be used for logs in the future.
     */
    const testing::TestInfo * const testInfo_ = ::testing::UnitTest::GetInstance()->current_test_info();
    
    /**
     *    @brief Setup for SomeIPRouter. Initializes required variables.
     */
    void SetUp() override {
         mockUCore_.init();
    }
    
    /**
     *    @brief Teardown for SomeIPRouter.
     */
    void TearDown() override {
    }

};
