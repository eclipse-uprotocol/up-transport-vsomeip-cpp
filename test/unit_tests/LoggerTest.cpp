#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Logger.hpp"
#include "spdlog/sinks/ostream_sink.h"

/**
 *  @brief Test fixture for Logger.
 */
class LoggerTests : public ::testing::Test {
protected:
    /**
     *  @brief Setup for Logger. Initializes required variables.
     */
    void SetUp() override {
    }

    /**
     *  @brief Teardown for Logger.
     */
    void TearDown() override {
    }
};

/**
 *  @brief Test for the logger setup when no logger ptr is passed.
 */
TEST_F(LoggerTests, SetupLoggerEmptyTest) {
    EXPECT_NO_THROW(setupLogger());
    auto logger = spdlog::default_logger();

    EXPECT_EQ("streamer-log", logger->name());
    EXPECT_EQ(spdlog::level::trace, logger->level());
}