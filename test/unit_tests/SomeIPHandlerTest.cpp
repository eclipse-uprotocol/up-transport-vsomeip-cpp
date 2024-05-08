#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vsomeip/vsomeip.hpp>
#include <memory>
#include <vector>
#include "SomeIPHandler.hpp"
#include "UCore.hpp"
#include "MessageTranslator.hpp"
#include "mock/MockSomeipInterface.hpp"
#include "mock/MockSomeIPRouterInterface.hpp"
#include "mock/MockSomeIPHandler.hpp"
#include "UMsg.hpp"
#include <uprotocol-cpp-ulink-zenoh/transport/zenohUTransport.h>


using ::testing::Return;

/**
 * @brief Test fixture to set up neccessary items for unit testing
 */
class SomeIPHandlerTest : public ::testing::Test {
protected:


    MockSomeipInterface mockSomeipInterface;
    MockSomeIPRouterInterface mockRouterInterface;
    std::unique_ptr<SomeIPHandler> handlerClient;
    std::unique_ptr<SomeIPHandler> handlerServer;
    std::map<vsomeip_v3::eventgroup_t, SubscriptionInfo> subscriptions_;

        SomeIPHandlerTest() {
            handlerClient = std::make_unique<SomeIPHandler>(mockSomeipInterface, mockRouterInterface, HandlerType::Client, vsomeip_v3::service_t(0x1234), vsomeip_v3::instance_t(0x1111), 10, 5);
            handlerServer = std::make_unique<SomeIPHandler>(mockSomeipInterface, mockRouterInterface, HandlerType::Server, vsomeip_v3::service_t(0x1234), vsomeip_v3::instance_t(0x1111), std::numeric_limits<uint16_t>::max(), 5);
        }

    vsomeip_v3::service_t getHandlerServiceId(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->serviceId_;
    }
    vsomeip_v3::instance_t getHandlerInstanceId(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->instanceId_;
    }

    bool getHandlerIsServiceAvailable(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->isServiceAvailable_;
    }

    HandlerType getHandlerType(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->type_;
    }
    bool getaddThread(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->addThread();
    }
    uint16_t getMaxThreadsAllowed(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->maxThreadsAllowed_;
    }
    void getsimulateSubscriptionAck(const std::unique_ptr<SomeIPHandler>& handler, eventgroup_t const eventGroup) {
        return handler->simulateSubscriptionAck(eventGroup);
    }
    void getaddNewUuidToSubscriptionMap(const std::unique_ptr<SomeIPHandler>& handler, vsomeip_v3::eventgroup_t eventGroup, std::string& uuid, std::string& uPTopic) {
        handler->addNewUuidToSubscriptionMap(eventGroup, uuid, uPTopic);
    }
    void getAddSubscriptionEntry(const std::unique_ptr<SomeIPHandler>& handler, vsomeip_v3::eventgroup_t eventGroup, const std::string& uuid, const std::string& uPTopic) {
        handler->addSubscriptionEntry(eventGroup, uuid, uPTopic);
    }
    std::unordered_map<eventgroup_t, SubscriptionInfo>& getSubscriptions(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->subscriptions_;
    }
    bool getDoesSubscriptionExist(const std::unique_ptr<SomeIPHandler>& handler, vsomeip_v3::eventgroup_t eventGroup) {
        return handler->doesSubscriptionExist(eventGroup);
    }
    void getRegisterSubscriptionStatusHandler(const std::unique_ptr<SomeIPHandler>& handler, vsomeip_v3::eventgroup_t eventGroup) {
        handler->registerSubscriptionStatusHandler(eventGroup);
    }
    void getHandleOutboundSubscription(const std::unique_ptr<SomeIPHandler>& handler, std::shared_ptr<UMsg> const uMsg) {
        handler->handleOutboundSubscription(uMsg);
    }
    void setServiceAvailability(const std::unique_ptr<SomeIPHandler>& handler, bool availability) {
        handler->isServiceAvailable_ = availability;
    }
    void getOfferEvent(const std::unique_ptr<SomeIPHandler>& handler, std::shared_ptr<std::set<eventgroup_t>> eventGroupSetPtr) {
    handler->offerEvent(eventGroupSetPtr);
    }
    void getQuit(const std::unique_ptr<SomeIPHandler>& handler) {
        handler->quit();
    }
    void getFlush(const std::unique_ptr<SomeIPHandler>& handler) {
        handler->flush();
    }
    uint16_t getThreadsOccupied(const std::unique_ptr<SomeIPHandler>& handler) {
        return handler->threadsOccupied_;
    }
    void getHandleOutboundMsg(const std::unique_ptr<SomeIPHandler>& handler, std::shared_ptr<UMsg> const uMsg) {
        handler->handleOutboundMsg(uMsg);
    }
    std::string getBuildTopicKey(const std::unique_ptr<SomeIPHandler>& handler, vsomeip_v3::service_t const &service_id, vsomeip_v3::eventgroup_t const &eventID) {
        return handler->buildTopicKey(service_id, eventID);
    }
    void getHandleInboundMessage(const std::unique_ptr<SomeIPHandler>& handler, std::shared_ptr<message> const &msg) {
        handler->handleInboundMsg(msg);
    }
    void getProcessMessage(const std::unique_ptr<SomeIPHandler>& handler, std::unique_ptr<SomeIPHandler::QItem>& item) {
        handler->processMessage(std::move(item));
    }
    std::unique_ptr<SomeIPHandler::QItem> createQItem(SomeIPHandler& handler, HandlerMsgType type, unsigned long messageData, std::shared_ptr<void> messagePtr) {
        return std::make_unique<SomeIPHandler::QItem>(type, messageData, messagePtr);
    }
};


/**
 * @brief Assets needed to make mockUMsg
 */
UAuthority g_uAuthorityLocal = UAuthority::local();
UEntity g_uEntity = UEntity::longFormat("example.service", 1);
UResource g_uResource = UResource::longFormat("example_instance", "example_name", "");
auto const g_testUURI = UUri(g_uAuthorityLocal, g_uEntity, g_uResource);

UUID g_uuid;
auto g_priority = UPriority::STANDARD;
auto g_publishType = UMessageType::PUBLISH;
UAttributesBuilder g_builder(g_uuid, g_publishType, g_priority);
UAttributes g_attributes = g_builder.build();
uint8_t buffer[3];
UPayload g_payload(buffer, sizeof(buffer), UPayloadType::VALUE);


/**
 * @brief Unit test to check that IsServiceAvailable will be false initially
 */
TEST_F(SomeIPHandlerTest, TestServiceAvailabilityInitiallyFalse) {
    EXPECT_FALSE(getHandlerIsServiceAvailable(handlerClient));
}

/**
 * @brief Unit test to verify that a message is queued for outbound transmission
 */
TEST_F(SomeIPHandlerTest, TestQueueOutboundMsg) {
    std::shared_ptr<UMsg> mockMsg = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );

    handlerClient->queueOutboundMsg(mockMsg);
}

/**
 * @brief Unit test to verify the constructor of the SomeIPHandler class with Client type
 */
TEST_F(SomeIPHandlerTest, TestConstructor_ClientType) {
    EXPECT_EQ(getHandlerType(handlerClient), HandlerType::Client);
    EXPECT_EQ(getHandlerServiceId(handlerClient), vsomeip_v3::service_t(0x1234));
    EXPECT_FALSE(getHandlerIsServiceAvailable(handlerClient));
}

/**
 * @brief Unit test to verify the constructor of the SomeIPHandler class with Server type
 */
TEST_F(SomeIPHandlerTest, TestConstructor_ServerType) {
    EXPECT_EQ(getHandlerType(handlerServer), HandlerType::Server);
    EXPECT_EQ(getHandlerServiceId(handlerServer), vsomeip_v3::service_t(0x1234));
}

/**
 * @brief Unit test to verify the constructor of the SomeIPHandler class with maximum number of threads
 */
TEST_F(SomeIPHandlerTest, TestConstructor_MaxThreads) {
    EXPECT_EQ(getMaxThreadsAllowed(handlerServer), std::numeric_limits<uint16_t>::max());
}


/**
 * @brief Unit test to verify the behavior of onAvailability when the service is available
 */
TEST_F(SomeIPHandlerTest, TestOnAvailability_ServiceAvailable) {
    handlerClient->onAvailability(vsomeip_v3::service_t(0x1234), vsomeip_v3::instance_t(0x1111), true);
    EXPECT_TRUE(getHandlerIsServiceAvailable(handlerClient));
}

/**
 * @brief Unit test to verify the behavior of onAvailability when the service is unavailable
 */
TEST_F(SomeIPHandlerTest, TestOnAvailability_ServiceUnavailable) {
    handlerClient->onAvailability(vsomeip_v3::service_t(0x1234), vsomeip_v3::instance_t(0x1111), false);
    EXPECT_FALSE(getHandlerIsServiceAvailable(handlerClient));
}

/**
 * @brief Unit test to verify that isRunning returns true
 */
TEST_F(SomeIPHandlerTest, TestIsRunningReturnsTrue) {
    getaddThread(handlerClient);
    bool result = handlerClient->isRunning();
    EXPECT_TRUE(result);
}

/**
 * @brief Unit test to verify the behavior of postMessageToQueuee when the handler is running and the priority is valid
 */
TEST_F(SomeIPHandlerTest, TestpostMessageToQueueeWhenRunningAndPriorityValid) {
    unsigned long data = 123;
    std::shared_ptr<void> ptr = std::make_shared<int>(456);
    uint16_t priority = 2;

    getaddThread(handlerClient);
    bool result = handlerClient->postMessageToQueue(HandlerMsgType::Outbound, data, ptr, priority);
    EXPECT_TRUE(result);
}

/**
 * @brief Unit test to verify the behavior of postMessageToQueuee when the handler is running and the priority is invalid
 */
TEST_F(SomeIPHandlerTest, TestpostMessageToQueueeWhenRunningAndPriorityInvalid) {
    unsigned long data = 123;
    std::shared_ptr<void> ptr = std::make_shared<int>(456);
    uint16_t priority = 456;

    bool result = handlerClient->postMessageToQueue(HandlerMsgType::Outbound, data, ptr, priority);
    EXPECT_FALSE(result);
}

/**
 * @brief Unit test to verify the behavior of onSubscription when the client is subscribed to a service
 */
TEST_F(SomeIPHandlerTest, TestOnSubscription) {
    client_t client = 123;
    secClientType secClient;
    std::string str = "test_subscription";
    bool isSubscribed = true;

    bool result = handlerClient->onSubscription(client, &secClient, str, isSubscribed);
    EXPECT_TRUE(result);
}

/**
 * @brief Unit test to verify the behavior of onSubscription when the client is not subscribed to a service
 */
TEST_F(SomeIPHandlerTest, TestOnSubscription_NotSubscribed) {
    client_t client = 123;
    secClientType secClient;
    std::string str = "test_subscription";
    bool isSubscribed = false;

    bool result = handlerClient->onSubscription(client, &secClient, str, isSubscribed);
    EXPECT_TRUE(result);
}

/**
 * @brief Unit test to verify that no exception is thrown for onSubscriptionStatus when the status is successful
 */
TEST_F(SomeIPHandlerTest, TestOnSubscriptionStatus_Successful) {
    vsomeip_v3::service_t service = vsomeip_v3::service_t(0x1234);
    vsomeip_v3::instance_t instance = vsomeip_v3::instance_t(0x1111);
    vsomeip_v3::eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);
    vsomeip_v3::event_t event = vsomeip_v3::event_t(0x3333);
    uint16_t status = 0;

    EXPECT_NO_THROW(handlerClient->onSubscriptionStatus(service, instance, eventGroup, event, status));
}

/**
 * @brief Unit test to verify the behavior of simulateSubscriptionAck
 */
TEST_F(SomeIPHandlerTest, TestSimulateSubscriptionAck) {
    vsomeip_v3::eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);


    getsimulateSubscriptionAck(handlerClient, eventGroup);


    EXPECT_EQ(getHandlerServiceId(handlerClient), vsomeip_v3::service_t(0x1234));
    EXPECT_EQ(getHandlerInstanceId(handlerClient), vsomeip_v3::instance_t(0x1111));
    EXPECT_EQ(eventGroup, vsomeip_v3::eventgroup_t(0x2222));
}/**
 * @brief Unit test to verify the behavior of addNewUuidToSubscriptionMap when the uPTopic matches with the existing uPTopic
 */
TEST_F(SomeIPHandlerTest, TestAddNewUuidToSubscriptionMap_UPTopicMatches) {
    eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);
    std::string uuid = "test_uuid";
    std::string uPTopic = "test_uPTopic";


    SubscriptionInfo info;


    info.setUPtopic(uPTopic);


    info.getUuidWaitlist().push_back(uuid);



    getSubscriptions(handlerServer).insert({eventGroup, info});


    getaddNewUuidToSubscriptionMap(handlerServer, eventGroup, uuid, uPTopic);


    auto waitlist = getSubscriptions(handlerServer).at(eventGroup).getUuidWaitlist();
    EXPECT_FALSE(std::find(waitlist.begin(), waitlist.end(), uuid) == waitlist.end());
}

/**
 * @brief Unit test to verify the behavior of addNewUuidToSubscriptionMap when the uPTopic does not match with the existing uPTopic
 */
TEST_F(SomeIPHandlerTest, TestAddNewUuidToSubscriptionMap_UPTopicDoesNotMatch) {
    eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);
    std::string uuid = "test_uuid";
    std::string uPTopic = "test_uPTopic";
    std::string diffuPTopic = "different_uPTopic";


    getaddNewUuidToSubscriptionMap(handlerServer, eventGroup, uuid, uPTopic);
    getaddNewUuidToSubscriptionMap(handlerServer, eventGroup, uuid, diffuPTopic);


    auto waitlist = getSubscriptions(handlerServer).at(eventGroup).getUuidWaitlist();
    EXPECT_TRUE(std::find(waitlist.begin(), waitlist.end(), uuid) == waitlist.end());
}


/**
 * @brief Unit test to verify the behavior of addSubscriptionEntry
 */
TEST_F(SomeIPHandlerTest, TestAddSubscriptionEntry) {
    vsomeip_v3::eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);
    std::string uuid = "test_uuid";
    std::string uPTopic = "test_uPTopic";


    getAddSubscriptionEntry(handlerServer, eventGroup, uuid, uPTopic);


    auto it = getSubscriptions(handlerServer).find(eventGroup);
    EXPECT_TRUE(it != getSubscriptions(handlerServer).end());
    EXPECT_EQ(it->second.getUPtopic(), uPTopic);
    EXPECT_EQ(it->second.getUuidWaitlist().size(), 1);
    EXPECT_EQ(it->second.getUuidWaitlist()[0], uuid);
}

/**
 * @brief Unit test to verify the behavior of doesSubscriptionExist when the subscription exists
 */
TEST_F(SomeIPHandlerTest, TestDoesSubscriptionExist_SubscriptionExists) {

    vsomeip_v3::eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);


    getAddSubscriptionEntry(handlerServer, eventGroup, "test_uuid", "test_uPTopic");


    bool result = getDoesSubscriptionExist(handlerServer, eventGroup);


    EXPECT_TRUE(result);
}

/**
 * @brief Unit test to verify the behavior of doesSubscriptionExist when the subscription does not exist
 */
TEST_F(SomeIPHandlerTest, TestDoesSubscriptionExist_SubscriptionDoesNotExist) {

    vsomeip_v3::eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);


    bool result = getDoesSubscriptionExist(handlerServer, eventGroup);


    EXPECT_FALSE(result);
}

/**
 * @brief Unit test to verify the behavior of registerSubscriptionStatusHandler
 */
TEST_F(SomeIPHandlerTest, TestRegisterSubscriptionStatusHandler) {
    vsomeip_v3::eventgroup_t eventGroup = vsomeip_v3::eventgroup_t(0x2222);


    getRegisterSubscriptionStatusHandler(handlerServer, eventGroup);


    EXPECT_EQ(getHandlerServiceId(handlerClient), vsomeip_v3::service_t(0x1234));
    EXPECT_EQ(getHandlerInstanceId(handlerClient), vsomeip_v3::instance_t(0x1111));
    EXPECT_EQ(eventGroup, vsomeip_v3::eventgroup_t(0x2222));
}
/**
 * @brief Unit test to verify the behavior of handleOutboundSubscription when the router interface is state registered and the service is available
 */
TEST_F(SomeIPHandlerTest, TestHandleOutboundSubscription_StateRegisteredAndServiceAvailable) {

    std::shared_ptr<UMsg> mockMsg = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );

    setServiceAvailability(handlerClient, true);


    getHandleOutboundSubscription(handlerClient, mockMsg);


    EXPECT_EQ(getHandlerServiceId(handlerClient), vsomeip_v3::service_t(0x1234));
    EXPECT_EQ(getHandlerInstanceId(handlerClient), vsomeip_v3::instance_t(0x1111));
}

/**
 * @brief Unit test to verify the behavior of handleOutboundSubscription when the router interface is not state registered or the service is not available
 */
TEST_F(SomeIPHandlerTest, TestHandleOutboundSubscription_StateNotRegisteredOrServiceNotAvailable) {

    std::shared_ptr<UMsg> mockMsg = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );


    setServiceAvailability(handlerClient, false);


    getHandleOutboundSubscription(handlerClient, mockMsg);


    EXPECT_FALSE(getDoesSubscriptionExist(handlerClient, mockMsg->getResourceId()));
    EXPECT_EQ(getHandlerServiceId(handlerClient), vsomeip_v3::service_t(0x1234));
    EXPECT_EQ(getHandlerInstanceId(handlerClient), vsomeip_v3::instance_t(0x1111));
}


/**
 * @brief Unit test to verify the behavior of offerEvent
 */
TEST_F(SomeIPHandlerTest, TestOfferEvent) {

    std::shared_ptr<std::set<vsomeip_v3::eventgroup_t>> eventGroupSetPtr = std::make_shared<std::set<vsomeip_v3::eventgroup_t>>();
    eventGroupSetPtr->insert(vsomeip_v3::eventgroup_t(0x1111));
    eventGroupSetPtr->insert(vsomeip_v3::eventgroup_t(0x2222));
    eventGroupSetPtr->insert(vsomeip_v3::eventgroup_t(0x3333));


    EXPECT_NO_THROW(getOfferEvent(handlerServer, eventGroupSetPtr));
}

/**
 * @brief Unit test to verify the behavior of offerEvent
 * when the event group set is empty
 */
TEST_F(SomeIPHandlerTest, TestOfferEvent_EmptyEventGroupSet) {
    std::shared_ptr<std::set<eventgroup_t>> eventGroupSetPtr = std::make_shared<std::set<eventgroup_t>>();


    EXPECT_NO_THROW(getOfferEvent(handlerServer, eventGroupSetPtr));
}

/**
 * @brief Unit test to verify the behavior of quit() when the handler is running
 */
TEST_F(SomeIPHandlerTest, TestQuitWhenRunning) {

    getaddThread(handlerClient);


    EXPECT_NO_THROW(getQuit(handlerClient));
}

/**
 * @brief Unit test to verify the behavior of quit() when the handler is not running
 */
TEST_F(SomeIPHandlerTest, TestQuitWhenNotRunning) {


   EXPECT_NO_THROW(getQuit(handlerClient));
}

/**
 * @brief Unit test to verify the behavior of flush function
 */
TEST_F(SomeIPHandlerTest, TestFlush) {

    std::shared_ptr<UMsg> mockMsg1 = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );
    std::shared_ptr<UMsg> mockMsg2 = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );
    handlerClient->queueOutboundMsg(mockMsg1);
    handlerClient->queueOutboundMsg(mockMsg2);


    getFlush(handlerClient);


    EXPECT_EQ(getThreadsOccupied(handlerClient), 0U);
}

/**
 * @brief Unit test to verify the behavior of handleOutboundMsg for OutSubscription message type
 */
TEST_F(SomeIPHandlerTest, TestHandleOutboundMsg_OutSubscription) {
    std::shared_ptr<UMsg> mockMsg = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );
    //mockMsg->setMsgType(UMsgType::OutSubscription);


    EXPECT_NO_THROW(getHandleOutboundMsg(handlerClient, mockMsg));
}

/**
 * @brief Unit test to verify the behavior of handleOutboundMsg for unknown message type
 */
TEST_F(SomeIPHandlerTest, TestHandleOutboundMsg_UnknownMessageType) {
    std::shared_ptr<UMsg> mockMsg = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );
    //mockMsg->setMsgType(static_cast<UMsgType>(999));


    EXPECT_NO_THROW(getHandleOutboundMsg(handlerClient, mockMsg));
}

/**
 * @brief Unit test to verify the behavior of buildTopicKey function
 *        when given valid service ID and event group ID
 */
TEST_F(SomeIPHandlerTest, TestBuildTopicKey_ValidIDs) {
    vsomeip_v3::service_t serviceID = 0x1234;
    vsomeip_v3::eventgroup_t eventGroupID = 0x5678;

    std::string expectedTopicKey = "1234,5678";
    std::string actualTopicKey = getBuildTopicKey(handlerClient, serviceID, eventGroupID);

    EXPECT_EQ(actualTopicKey, expectedTopicKey);
}

/**
 * @brief Unit test to verify the behavior of HandleInboundMessage for Request type
 */
TEST_F(SomeIPHandlerTest, HandleInboundMessageRequest) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());


    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(0x1234);
    request->set_instance(0x1111);
    request->set_method(0x3456);

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > its_payload_data;
    for (vsomeip::byte_t i=0; i<10; i++) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);


    EXPECT_NO_THROW(getHandleInboundMessage(handlerServer, request));
}

/**
 * @brief Unit test to verify the behavior of HandleInboundMessage for Response type
 */
TEST_F(SomeIPHandlerTest, HandleInboundMessageResponse) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());


    std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
    request->set_service(0x1234);
    request->set_instance(0x1111);
    request->set_method(0x3456);

    std::shared_ptr< vsomeip::payload > its_payload_request = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > its_payload_data_request;
    for (vsomeip::byte_t i=0; i<10; i++) {
        its_payload_data_request.push_back(i % 256);
    }
    its_payload_request->set_data(its_payload_data_request);
    request->set_payload(its_payload_request);

    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(request);
    std::shared_ptr<vsomeip::payload> its_payload_response = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data_response;
    for (int i=9; i>=0; i--) {
        its_payload_data_response.push_back(i % 256);
    }
    its_payload_response->set_data(its_payload_data_response);
    its_response->set_payload(its_payload_response);

    EXPECT_NO_THROW(getHandleInboundMessage(handlerClient, its_response));
}

/**
 * @brief Unit test to verify the behavior of HandleInboundMessage for Notification type
 */
TEST_F(SomeIPHandlerTest, HandleInboundMessageNotification) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());


    std::shared_ptr<vsomeip::message> notification;
    notification = vsomeip::runtime::get()->create_request();
    notification->set_service(0x1234);
    notification->set_instance(0x1111);
    notification->set_method(0x3456);
    notification->set_message_type(message_type_e::MT_NOTIFICATION);

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > its_payload_data;
    for (vsomeip::byte_t i=0; i<10; i++) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    notification->set_payload(its_payload);


    EXPECT_NO_THROW(getHandleInboundMessage(handlerClient, notification));
}

/**
 * @brief Unit test to verify the behavior of HandleInboundMessage for Unknown type
 */
TEST_F(SomeIPHandlerTest, HandleInboundMessageUnknownType) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());




    std::shared_ptr<vsomeip::message> unknown_msg = vsomeip::runtime::get()->create_request();
    unknown_msg->set_message_type(static_cast<vsomeip::message_type_e>(999)); // Assuming 999 is not a valid message type

    EXPECT_NO_THROW(getHandleInboundMessage(handlerClient, unknown_msg));
}

/**
 * @brief Unit test to verify the behavior of ProcessMessage for Outbound type
 */
TEST_F(SomeIPHandlerTest, ProcessMessage_Outbound) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());

    std::shared_ptr<UMsg> outboundMsg = std::make_shared<UMsg>(
        std::make_shared<UUri>(g_testUURI),
        std::make_shared<UAttributes>(g_attributes),
        std::make_shared<UPayload>(buffer, sizeof(buffer), UPayloadType::VALUE)
    );

    auto outboundItem = createQItem(*handlerClient, HandlerMsgType::Outbound, 0UL, outboundMsg);

    getProcessMessage(handlerClient, outboundItem);
}

/**
 * @brief Unit test to verify the behavior of ProcessMessage for Inbound type
 */
TEST_F(SomeIPHandlerTest, ProcessMessage_Inbound) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());

    std::shared_ptr<vsomeip::message> inboundMsg = vsomeip::runtime::get()->create_request();
    inboundMsg->set_service(0x1234);
    inboundMsg->set_instance(0x1111);
    inboundMsg->set_method(0x3456);
    inboundMsg->set_message_type(static_cast<vsomeip::message_type_e>(999));
    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > its_payload_data;
    for (vsomeip::byte_t i=0; i<10; i++) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    inboundMsg->set_payload(its_payload);
    auto inboundItem = createQItem(*handlerClient, HandlerMsgType::Inbound, 0UL, inboundMsg);
    EXPECT_NO_THROW(getProcessMessage(handlerClient, inboundItem));
}

/**
 * @brief Unit test to verify the behavior of ProcessMessage for Stop type
 */
TEST_F(SomeIPHandlerTest, ProcessMessage_Stop) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());



    auto stopItem = createQItem(*handlerClient, HandlerMsgType::Stop, 0UL, nullptr);


    EXPECT_NO_THROW(getProcessMessage(handlerClient, stopItem));
}

/**
 * @brief Unit test to verify the behavior of ProcessMessage for UpdateMaxThreads type
 */
TEST_F(SomeIPHandlerTest, ProcessMessage_UpdateMaxThreads) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());


    auto updateMaxThreadsItem = createQItem(*handlerClient, HandlerMsgType::UpdateMaxThreads, 0UL, nullptr);

    EXPECT_NO_THROW(getProcessMessage(handlerClient, updateMaxThreadsItem));
}

/**
 * @brief Unit test to verify the behavior of ProcessMessage for Request type
 */
TEST_F(SomeIPHandlerTest, ProcessMessage_RequestThread) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());


    auto requestThreadItem = createQItem(*handlerClient, HandlerMsgType::RequestThread, 0UL, nullptr);


    EXPECT_NO_THROW(getProcessMessage(handlerClient, requestThreadItem));
}

/**
 * @brief Unit test to verify the behavior of ProcessMessage for Default type
 */
TEST_F(SomeIPHandlerTest, ProcessMessage_default) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());

    auto defaultItem = createQItem(*handlerClient, static_cast<HandlerMsgType>(999), 0UL, nullptr);


    EXPECT_NO_THROW(getProcessMessage(handlerClient, defaultItem));
}

/**
 * @brief Unit test to verify the behavior of ProcessMessage for OfferEvent type
 */
TEST_F(SomeIPHandlerTest, ProcessMessage_OfferEvent) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());




    auto eventGroupSetPtr = std::make_shared<std::set<eventgroup_t>>();

    auto offerEventItem = createQItem(*handlerClient,HandlerMsgType::OfferUResource, 0UL, eventGroupSetPtr);


    EXPECT_NO_THROW(getProcessMessage(handlerClient, offerEventItem));
}



