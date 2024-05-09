#include "gtest/gtest.h"
#include "SomeipWrapper.hpp"
#include "mock/CustomEventListener.hpp"


TEST(SampleTest, Equality) {
    EXPECT_EQ(1, 1);
}

/**
 *  @brief main test function. It initializes the listener for all testing.
 */
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // Gets hold of the event listener list.
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    // Adds a listener to the end. Google Test takes the ownership.
    listeners.Append(new CustomEventListener);

    return RUN_ALL_TESTS();
}