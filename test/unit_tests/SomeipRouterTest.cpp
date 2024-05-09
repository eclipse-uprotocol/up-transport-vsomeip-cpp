#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vsomeip/vsomeip.hpp>
#include <memory>
#include <vector>
#include "SomeipRouter.hpp"
#include "MessageTranslator.hpp"
#include "mock/UURIHelper.hpp"
#include <VsomeipUTransport.hpp>
#include <up-cpp/transport/builder/UAttributesBuilder.h>
#include <up-core-api/uri.pb.h>
#include <up-cpp/uuid/factory/Uuidv8Factory.h>
#include <up-cpp/uuid/serializer/UuidSerializer.h>

using ::testing::Return;
using namespace uprotocol::utransport;
using namespace uprotocol::uuid;
using namespace uprotocol::v1;
using ::testing::NiceMock;

/**
 *  @brief Mock UListener class.
 */
class MockUListener : public UListener {
public:
    MOCK_METHOD(UStatus, onReceive, (UMessage &message), (const, override));
};

/**
 *  @brief SomeipRouter test fixture.
 */
class SomeipRouterTests : public testing::Test {
protected:
    /**
     *  @brief Mock UListener object needed to initialize a SomeipRouter.
     */
    MockUListener mockListener;
    /**
     *  @brief Message for routing.
     */
    UMessage umsg;
    /**
     *  @brief SomeipRouter object for testing.
     */
    // std::shared_ptr<SomeipRouter> router;

    /**
     *  @brief Setup for SomeipRouter.
     */
    void SetUp() override {
        //router = std::make_shared<SomeipRouter>(mockListener);
    }

    /**
     *  @brief Teardown for SomeipRouter.
     */
    void TearDown() override {
    }
};

/**
 *  @brief Build a UMessage needed for testing.
 */
UMessage buildUMessage(UMessageType type, UPriority priority) {
    auto uuid = Uuidv8Factory::create();
    std::shared_ptr<UUri> uRI = buildUURI();
    UAttributesBuilder builder(*uRI, uuid, type, priority);
    builder.setSink(*uRI);
    UAttributes attributes = builder.build();
    uint8_t buffer[10] = {0}; 
    UPayload payload(buffer, sizeof(buffer), UPayloadType::VALUE);

    UMessage umsg(payload, attributes);

    return umsg;
}

/**
 *  @brief Verify routeInboundSubscription() returns true.
 */
// TEST_F(SomeipRouterTests, TestRouteInboundSubscription) {
//     std::shared_ptr<SomeipRouter> router = std::make_shared<SomeipRouter>(mockListener);
//     std::string strTopic = "test_topic";
//     bool isSubscribe = true;

//     bool result = true;//router->routeInboundSubscription(strTopic, isSubscribe);

//     EXPECT_TRUE(result);
// }

/**
*  @brief Verify init() successfully initializes a SomeipRouter object.
*/
// TEST_F(SomeipRouterTests, routeInboundMsgTest) {
//     uprotocol::utransport::UMessage umsg;
//     EXPECT_CALL(mockListener,  onReceive(::testing::_)).Times(1);
//     EXPECT_TRUE(router->routeInboundMsg(umsg));
// }