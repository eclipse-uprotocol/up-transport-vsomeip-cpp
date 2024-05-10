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
#include "SomeipWrapper.hpp"
#include "MockApplication.hpp"
#include "MockSomeipInterface.hpp"
#include "MockSomeipRouterInterface.hpp"
#include "MockSomeipHandler.hpp"

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
     *  @brief SomeipWrapper object for testing.
     */
    std::shared_ptr<SomeipWrapper> someIpWrapperInstance;
    /**
     *  @brief Mock Application object needed to initialize a SomeipWrapper.
     */
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp =
        std::make_shared<::testing::NiceMock<MockApplication>>();
    /**
     *  @brief SomeipRouter object for testing.
     */
    std::shared_ptr<SomeipRouter> router;

    /**
     *  @brief Setup for SomeipRouter.
     */
    void SetUp() override {
        someIpWrapperInstance = std::make_shared<SomeipWrapper>();
        someIpWrapperInstance->setApp(mockApp);
        router = std::make_shared<SomeipRouter>(mockListener, someIpWrapperInstance);
    }

    /**
     *  @brief Teardown for SomeipRouter.
     */
    void TearDown() override {
    }

    std::shared_ptr<SomeipHandler> getHandler(uint16_t UEId){
        return router->getHandler(UEId);
    }
    void offerServicesAndEvents(std::shared_ptr<uprotocol::v1::UUri> uri){
        return router->offerServicesAndEvents(uri);
    }
    std::shared_ptr<std::vector<std::shared_ptr<uprotocol::v1::UUri>>> getUriList(const std::string &serviceType){
        return router->getUriList(serviceType);
    }

    stateType getState(){
        return router->state_;
    }
    void addHandler(std::shared_ptr<SomeipHandler> mockHandler, uint16_t mockHandlerKey) {
        router->handlers_[mockHandlerKey] = mockHandler;
    }

    void getOfferServicesAndEvents(std::shared_ptr<UUri> uriPtr) {
        router->offerServicesAndEvents(uriPtr);
    }
};

/**
 *  @brief Build a UMessage needed for testing.
 */
UMessage buildUMessage(UMessageType type, UPriority priority) {
    auto uuid = Uuidv8Factory::create();
    std::shared_ptr<UUri> uRI = buildUURI();
    std::shared_ptr<UUri> uRISource = buildUURI();
    UAttributesBuilder builder(*uRI, uuid, type, priority);
    builder.setSink(*uRI);
    builder.setSource(*uRISource);
    UAttributes attributes = builder.build();
    uint8_t buffer[10] = {0}; 
    UPayload payload(buffer, sizeof(buffer), UPayloadType::VALUE);

    UMessage umsg(payload, attributes);

    return umsg;
}

/**
 *  @brief Test init for SomeipRouter.
 */
TEST(RouterStandalone, initTest){
    std::shared_ptr<SomeipWrapper> someIpWrapperInstance = std::make_shared<SomeipWrapper>();
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp =
        std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance->setApp(mockApp);

    MockUListener listener; 
    SomeipRouter router(listener, someIpWrapperInstance);

    router.init();
}

// TEST_F(SomeipRouterTests, TestGetUriList) {

// std::string serviceType = "remoteServices";

// auto actualUriList = getUriList(serviceType);

// ASSERT_NE(actualUriList, nullptr);
// EXPECT_EQ(actualUriList->size(), 2);

// }

/**
 *  @brief Verify that state is correctly set by onState.
 */
TEST_F(SomeipRouterTests, TestOnState) {
    router->onState(state_type_e::ST_REGISTERED);

    EXPECT_EQ(getState(), state_type_e::ST_REGISTERED);
}

/**
 *  @brief Check that isStateRegistered returns the correct state.
 */
TEST_F(SomeipRouterTests, TestisStateRegistered) {
    router->onState(state_type_e::ST_REGISTERED);

    EXPECT_TRUE(router->isStateRegistered());
}

/**
 *  @brief Check that inbound subscription is routed correctly.
 */
TEST_F(SomeipRouterTests, TestRouteInboundSubscription) {
    std::string strTopic = "test_topic";
    bool isSubscribe = true;
    bool result = router->routeInboundSubscription(strTopic, isSubscribe);

    EXPECT_TRUE(result);
}

/**
*  @brief Test that verifies routeInboundMsg routes the message.
*/
TEST_F(SomeipRouterTests, routeInboundMsgTest) {
    uprotocol::utransport::UMessage umsg;
    EXPECT_CALL(mockListener,  onReceive(::testing::_)).Times(1);
    EXPECT_TRUE(router->routeInboundMsg(umsg));
}

/**
 *  @brief Test that the message translator object is returned.
 */
TEST_F(SomeipRouterTests, TestGetMessageTranslator) {
    MessageTranslator& translator = router->getMessageTranslator();

    ASSERT_NE(&translator, nullptr);

}

/**
 *  @brief Verify that a publish or response outbound message is not routed if the handler does not exist in the list.
 */
TEST_F(SomeipRouterTests, routeOutboundMsgPublishAndResponseNullTest) {
    uprotocol::utransport::UMessage umsg = buildUMessage(UMessageType::UMESSAGE_TYPE_PUBLISH,UPriority::UPRIORITY_CS4);

    EXPECT_CALL(mockListener,  onReceive(::testing::_)).Times(1);

    EXPECT_FALSE(router->routeOutboundMsg(umsg));
    EXPECT_TRUE(router->routeInboundMsg(umsg));
    EXPECT_FALSE(router->routeOutboundMsg(umsg));
}

/**
 *  @brief Test that getOfferServicesAndEvents does not manipulate a SomeipHandler if there is no corresponding
 *  entity ID.
 */

TEST_F(SomeipRouterTests, offerServicesAndEventsTest) {
    MockSomeipInterface mockSomeipInterface;
    MockSomeipRouterInterface mockSomeipRouterInterface;
    std::string const uEntityName        = "0x1102";
    uint32_t const uEntityVersionMajor   = 0x1; //Major Version
    uint32_t const uEntityVersionMinor   = 0x0; //Minor Version
    std::string const uAuthorityIp       = "172.17.0.1";
    uint16_t const uResourceId           = 0x0102; //Method ID
    std::string const uResourceName      = "rpc";
    std::string const uResourceInstance  = "0x0102";
    std::shared_ptr<UUri> uriPtr = std::make_shared<UUri>();

    UEntity uEntity;
    UAuthority uAuthority;
    UResource uResource;

    uEntity.set_name(uEntityName.c_str());
    uEntity.set_version_major(uEntityVersionMajor);
    uEntity.set_version_minor(uEntityVersionMinor);

    uriPtr->mutable_entity()->CopyFrom(uEntity);

    uResource.set_id(uResourceId);
    uResource.set_name(uResourceName.c_str());
    uResource.set_instance(uResourceInstance);

    uriPtr->mutable_resource()->CopyFrom(uResource);

    std::shared_ptr<MockSomeipHandler> mockHandler = std::make_shared<MockSomeipHandler>(mockSomeipInterface,
                                                                                         mockSomeipRouterInterface,
                                                                                         HandlerType::Client,
                                                                                         uEntity,
                                                                                         uAuthority,
                                                                                         0x0102,
                                                                                         DEFAULT_PRIORITY_LEVELS);

    uint16_t mockHandlerKey = 0x1102;
    addHandler(mockHandler, mockHandlerKey);
    EXPECT_NO_THROW(getOfferServicesAndEvents(uriPtr));
}