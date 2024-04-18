#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>
#include "mock/MockMessageTranslator.hpp"
#include "Logger.h"
#include "mock/MockUCore.hpp"

/**
 *    @brief Test fixture for MessageTranslator.
 */
class MessageTranslatorTests : public testing::Test {
protected:
    /**
     *    @brief UCore object to initialize other objects.
     */
    MockUCore mockUCore_;
    /**
     *    @brief Create MessageTranslator object for testing.
     */
    MockMessageTranslator* mockMT_ = nullptr;
    /**
     *    @brief Holds test log info. Will be used for logs in the future.
     */
    const testing::TestInfo * const testInfo_ = ::testing::UnitTest::GetInstance()->current_test_info();
    
    /**
     *    @brief Setup for MessageTranslator. Initializes required variables.
     */
    void SetUp() override {
         mockUCore_.init();
    }
    
    /**
     *    @brief Teardown for MessageTranslator.
     */
    void TearDown() override {
    }
};
