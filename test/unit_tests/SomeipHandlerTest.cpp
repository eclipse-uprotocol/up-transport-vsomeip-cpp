#include "mock/SomeipHandlerTest.hpp"

/**
 *  @brief Create a UUri object for testing.
 */
auto const g_testUURI = buildUURI();

/**
 *  @brief payload for message.
 */
static uint8_t g_data[4] = "100";
UPayload g_payloadForHandler(g_data, sizeof(g_data), UPayloadType::VALUE);
/**
 *  @brief UMessage used for testing.
 */
UMessage g_messageHandler(g_payloadForHandler, createUAttributes());
/**
 *  @brief instance used for server, client, and standalone tests of SomeipHandler.
 */
instance_t const g_instance = 0x1111;

/**
 *  @brief Setup for SomeipHandler. Initializes required variables.
 */
void SomeipHandlerServerTests::SetUp() {
    handlerServer = std::make_unique<SomeipHandler>(mockSomeipInterface,
                                                    mockRouterInterface,
                                                    HandlerType::Server,
                                                    uEntity,
                                                    uAuthority,
                                                    g_instance,
                                                    2U);
}

/**
 *  @brief Teardown for SomeipHandler.
 */
void SomeipHandlerServerTests::TearDown() {
    handlerServer->quit();
}

/**
 *  @brief Setup for SomeipHandler. Initializes required variables.
 */
void SomeipHandlerClientTests::SetUp() {
    handlerClient = std::make_unique<SomeipHandler>(mockSomeipInterface,
                                                    mockRouterInterface,
                                                    HandlerType::Client,
                                                    uEntity,
                                                    uAuthority,
                                                    g_instance,
                                                    2U);
}

/**
 *  @brief Teardown for SomeipHandler.
 */
void SomeipHandlerClientTests::TearDown() {
    handlerClient->quit();
}

/**
 *  @brief Create a UResource object.
 */
std::unique_ptr<UResource> createUResource() {
    uint16_t const uResourceId = 0x0102; //Method ID
    std::string const uResourceName = "rpc";
    std::string const uResourceInstance = "0x0102";

    std::unique_ptr<UResource> uResource = std::make_unique<UResource>();
    uResource->set_id(uResourceId);
    uResource->set_name(uResourceName.c_str());
    uResource->set_instance(uResourceInstance);

    return uResource;
}

/**
 *  @brief Build a UAttributes object.
 */
UAttributes createUAttributes(UMessageType type) {
    auto uuid = Uuidv8Factory::create();
    auto const uPriority = UPriority::UPRIORITY_CS4;
    auto uPublishType = type;
    UAttributesBuilder uAttributesBuilder(*g_testUURI, uuid, uPublishType, uPriority);
    uAttributesBuilder.setReqid(uuid);

    return uAttributesBuilder.build();
}

/**
 *  @brief Create a message.
 */
std::shared_ptr<vsomeip::message>  createMessage() {
    std::shared_ptr<vsomeip::message> message;
    message = vsomeip::runtime::get()->create_request();
    message->set_service(0x1234);
    message->set_instance(g_instance);
    message->set_method(0x0102);
    message->set_client(0x0123);
    message->set_session(0x0456);
 
    std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > payloadData;
    for (vsomeip::byte_t i = 0; i < 10; i++) {
        payloadData.push_back(i % 256);
    }
    payload->set_data(payloadData);
    message->set_payload(payload);

    return message;
}

/**
 *  @brief Test to ensure the SomeipHandler constructor executes properly for client handlers.
 */
TEST(SomeipHandlerStandaloneTests, someipHandlerClientConstructorTest) {
    ::testing::NiceMock<MockSomeipInterface> mockSomeipInterface;
    ::testing::NiceMock<MockSomeipRouterInterface> mockRouterInterface;
    std::unique_ptr<SomeipHandler> handlerClient;
    UEntity uEntity;
    UAuthority uAuthority;

    ON_CALL(mockRouterInterface, isStateRegistered()).WillByDefault(Return(true));
    EXPECT_CALL(mockSomeipInterface, registerAvailabilityHandler(0,
                                                                 0,
                                                                 _,
                                                                 _,
                                                                 _)).Times(1);
    EXPECT_CALL(mockSomeipInterface, requestService(0, 0, _, _)).Times(1);
    EXPECT_CALL(mockSomeipInterface, offerService(0, 0, _, _)).Times(0);

    handlerClient = std::make_unique<SomeipHandler>(mockSomeipInterface,
                                                    mockRouterInterface,
                                                    HandlerType::Client,
                                                    uEntity,
                                                    uAuthority,
                                                    g_instance,
                                                    2U);
}

/**
 *  @brief Test to ensure the SomeipHandler constructor executes properly for server handlers.
 */
TEST(SomeipHandlerStandaloneTests, someipHandlerServerConstructorTest) {
    ::testing::NiceMock<MockSomeipInterface> mockSomeipInterface;
    ::testing::NiceMock<MockSomeipRouterInterface> mockRouterInterface;
    std::unique_ptr<SomeipHandler> handlerServer;
    UEntity uEntity;
    UAuthority uAuthority;

    EXPECT_CALL(mockSomeipInterface, registerAvailabilityHandler(0,
                                                                 0,
                                                                 _,
                                                                 _,
                                                                 _)).Times(0);
    EXPECT_CALL(mockSomeipInterface, requestService(0, 0, _, _)).Times(0);
    EXPECT_CALL(mockSomeipInterface, offerService(0, 0, _, _)).Times(1);

    handlerServer = std::make_unique<SomeipHandler>(mockSomeipInterface,
                                                    mockRouterInterface,
                                                    HandlerType::Server,
                                                    uEntity,
                                                    uAuthority,
                                                    g_instance,
                                                    2U);
}

/**
 *  @brief Verify the behavior of onSubscription when the client is subscribed to a service.
 */
TEST_F(SomeipHandlerClientTests, onSubscriptionTest) {
    client_t client = 123;
    secClientType secClient;
    std::string str = "test_subscription";
    bool isSubscribed = true;


    bool result = handlerClient->onSubscription(client, &secClient, str, isSubscribed);
    EXPECT_TRUE(result);
}

/**
 *  @brief Verify the behavior of onSubscription when the client is not subscribed to a service.
 */
TEST_F(SomeipHandlerClientTests, onSubscriptionNotSubscribedTest) {
    client_t client = 123;
    secClientType secClient;
    std::string str = "test_subscription";
    bool isSubscribed = false;

    bool result = handlerClient->onSubscription(client, &secClient, str, isSubscribed);
    EXPECT_TRUE(result);
}

/**
 *  @brief Verify the behavior of onSubscription when the client is subscribed to a service.
 */
TEST_F(SomeipHandlerClientTests, onSubscriptionStatusSubscribedPostsMessageToQueueTest) {
    eventgroup_t eventGroup = 0x5678;
    event_t event = 0x0001;
    uint16_t status = 0;

    setRunning(true);
    handlerClient->onSubscriptionStatus(this->service, g_instance, eventGroup, event, status);
    EXPECT_EQ(getQueueSize(), 1);
}

/**
 *  @brief Verify the behavior of onSubscription when the client is not subscribed to a service.
 */
TEST_F(SomeipHandlerClientTests, onSubscriptionStatusUnsubscribedPostsMessageToQueueTest) {
    eventgroup_t eventGroup = 0x5678;
    event_t event = 0x0001;
    uint16_t status = 1;

    setRunning(true);
    handlerClient->onSubscriptionStatus(this->service, g_instance, eventGroup, event, status);
    EXPECT_EQ(getQueueSize(), 1);
}

/**
 * @brief Ensure doesInboundSubscriptionExist returns true only when inbound subscription exists.
 */
TEST_F(SomeipHandlerClientTests, doesInboundSubscriptionExistTest) {
    /**
     *  @brief SubscriptionStatus object. Used to add to the subscriber count using handleInboundSubscription.
     */
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);

    /**
     *  @brief Create a copy of UResource object.
     */
    EXPECT_FALSE(getDoesInboundSubscriptionExist(this->eventGroup));

    /**
     *  @brief Add the resourceId and resource to the offeredResources_ map.
     */
    gethandleOfferUResource(g_testUURI);
    /**
     *  @brief Find resourceId in map and add to subscriber count.
     */
    gethandleInboundSubscription(subStatusPtr);
    EXPECT_TRUE(getDoesInboundSubscriptionExist(this->eventGroup));
}

/**
 *  @brief Check if the queue size increases when actOnBehalfOfSubscriptionAck is called.
 *  actOnBehalfOfSubscriptionAck calls onSubscriptionStatus which then calls postMessageToQueue.
 */
TEST_F(SomeipHandlerClientTests, actOnBehalfOfSubscriptionAckTest) {
    size_t originalSize = getQueueSize();

    getActOnBehalfOfSubscriptionAck(this->eventGroup);
    EXPECT_EQ(originalSize + 1, getQueueSize());
}


/**
 *  @brief Ensure addSubscriptionForRemoteService adds a subscription for a remote service.
 */
TEST_F(SomeipHandlerClientTests, addSubscriptionForRemoteServiceTest) {
    UResourceId_t resourceId = 0x0102;
    std::unique_ptr<UResource> resource = createUResource();
    std::shared_ptr<ResourceInformation> resourceInfo = std::make_shared<ResourceInformation>(*resource);

    EXPECT_TRUE(getaddSubscriptionForRemoteService(resourceId, resourceInfo));
}

/**
 *  @brief Ensure removeSubscriptionForRemoteService removes a subscription for a remote service.
 */
TEST_F(SomeipHandlerClientTests, removeSubscriptionForRemoteServiceTest) {
    UResourceId_t resourceId = 0x0102;
    std::unique_ptr<UResource> resource = createUResource();
    std::shared_ptr<ResourceInformation> resourceInfo = std::make_shared<ResourceInformation>(*resource);

    std::ignore = getaddSubscriptionForRemoteService(resourceId, resourceInfo);
    EXPECT_TRUE(getRemoveSubscriptionForRemoteService(resourceId));
}

/**
 *  @brief Verify that doesSubscriptionForRemoteServiceExist correctly returns if a remote subscription exists.
 */
TEST_F(SomeipHandlerClientTests, doesSubscriptionForRemoteServiceExistTest) {
    eventgroup_t eventGroup = 0x0123;
    UResourceId_t resourceId = 0x0123;
    std::unique_ptr<UResource> resource = createUResource();
    std::shared_ptr<ResourceInformation> resourceInfo = std::make_shared<ResourceInformation>(*resource);

    EXPECT_FALSE(getdoesSubscriptionForRemoteServiceExist(eventGroup));

    std::ignore = getaddSubscriptionForRemoteService(resourceId, resourceInfo);
    EXPECT_TRUE(getdoesSubscriptionForRemoteServiceExist(eventGroup));
}

/**
 *  @brief Check that SomeipHandler calls SomeipInterface.registerMessageHandler.
 */
TEST_F(SomeipHandlerClientTests, registerMessageHandlerTest) {
    EXPECT_CALL(mockSomeipInterface, registerMessageHandler(_,
                                                            _,
                                                            _,
                                                            _)).Times(1);
    handlerClient = std::make_unique<SomeipHandler>(mockSomeipInterface,
                                                mockRouterInterface,
                                                HandlerType::Client,
                                                uEntity,
                                                uAuthority,
                                                g_instance,
                                                2U);
}

/**
 *  @brief Verify that a UResource is offered when handleOfferUResource is called.
 */
TEST_F(SomeipHandlerClientTests, handleOfferUResourceTest) {
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);

    EXPECT_FALSE(getDoesInboundSubscriptionExist(this->eventGroup));

    gethandleOfferUResource(g_testUURI);
    gethandleInboundSubscription(subStatusPtr);
    EXPECT_TRUE(getDoesInboundSubscriptionExist(this->eventGroup));
}

/**
 *  @brief Test that the handleOutboundResponse method only calls SomeipInterface.send if the UUID exists in the map.
 */
TEST_F(SomeipHandlerClientTests, handleOutboundResponseTest) {
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(g_messageHandler);
    std::shared_ptr<vsomeip::message> message = createMessage();
    UMessage const &uMsg = *uMsgPtr;
    std::string strUUID = uprotocol::uuid::UuidSerializer::serializeToString(uMsg.attributes().reqid());

    EXPECT_CALL(mockSomeipInterface, send(_)).Times(0);
    getHandleOutboundResponse(uMsgPtr);
    addToUuidToSomeipRequestLookup(strUUID, message);

    EXPECT_CALL(mockSomeipInterface, send(_)).Times(1);
    getHandleOutboundResponse(uMsgPtr);
}

/**
 *  @brief Unit test to verify the behavior of HandleOutboundMsg for RESPONSE type.
 */
TEST_F(SomeipHandlerClientTests, handleOutboundMsgResponseTest) {
    auto const type = UMessageType::UMESSAGE_TYPE_RESPONSE;
    auto uuid = Uuidv8Factory::create();
    UAttributesBuilder builderHandler(*g_testUURI,uuid, type, UPriority::UPRIORITY_CS4);
    UAttributes attributesHandler = builderHandler.build();
    auto uMsg = std::make_shared<UMessage>(g_payloadForHandler, attributesHandler);
    std::string strUUID = uprotocol::uuid::UuidSerializer::serializeToString(uMsg->attributes().reqid());
    std::shared_ptr<vsomeip::message> message = createMessage();

    addToUuidToSomeipRequestLookup(strUUID, message);
    EXPECT_CALL(mockSomeipInterface, send(_)).Times(1);
    EXPECT_NO_THROW(getHandleOutboundMsg(uMsg));
}

/**
 *  @brief Unit test to verify the behavior of HandleOutboundMsg for UNKNOWN type
 */
TEST_F(SomeipHandlerClientTests, HandleOutboundMsgUnknownTypeTest) {
    auto const type = static_cast<UMessageType>(999);
    auto uuid = Uuidv8Factory::create();
    UAttributesBuilder builderHandler(*g_testUURI,uuid, type, UPriority::UPRIORITY_CS2);
    UAttributes attributesHandler = builderHandler.build();
    auto uMsg = std::make_shared<UMessage>(g_payloadForHandler, attributesHandler);
    std::shared_ptr<vsomeip::message> message = createMessage();
    std::string strUUID = uprotocol::uuid::UuidSerializer::serializeToString(uMsg->attributes().reqid());

    addToUuidToSomeipRequestLookup(strUUID, message);
    EXPECT_CALL(mockSomeipInterface, createPayload()).Times(0);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(0);
    EXPECT_CALL(mockSomeipInterface, send(_)).Times(0);

    EXPECT_NO_THROW(getHandleOutboundMsg(uMsg));
}

/**
 *  @brief Unit test to verify the behavior of HandleOutboundMsg for REQUEST type
 */
TEST_F(SomeipHandlerClientTests, HandleOutboundMsgRequestTest) {
    auto const type = UMessageType::UMESSAGE_TYPE_REQUEST;
    auto uuid = Uuidv8Factory::create();
    UAttributesBuilder builder(*g_testUURI,uuid, type, UPriority::UPRIORITY_CS4);
    builder.setSink(*g_testUURI);
    UAttributes uAttributes = builder.build();
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);
    auto uMsg = std::make_shared<UMessage>(g_payloadForHandler, uAttributes);
    std::shared_ptr<vsomeip::message> message = createMessage();
    std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();

    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockInterface, createRequest()).Times(1).WillOnce(Return(message));
    EXPECT_CALL(mockInterface, createPayload()).Times(1).WillOnce(Return(payload));
    getHandleOutboundMsg(uMsg);
}

/**
 *  @brief Unit test to verify the behavior of postMessageToQueuee when the handler is running
 *  and the priority is invalid.
 */
TEST_F(SomeipHandlerClientTests, postMessageToQueueWhenRunningAndPriorityInvalidTest) {
    unsigned long data = 123;
    std::shared_ptr<void> ptr = std::make_shared<int>(456);
    uint16_t priority = 456;

    bool result = handlerClient->postMessageToQueue(HandlerMsgType::Outbound, data, ptr, priority);
    EXPECT_FALSE(result);
}

/**
 *  @brief Unit test to verify the behavior of ProcessMessage for Inbound type.
 */
TEST_F(SomeipHandlerClientTests, processMessageInboundTest) {
    std::shared_ptr<vsomeip::message> message = createMessage();
    message->set_message_type(static_cast<vsomeip::message_type_e>(message_type_e::MT_REQUEST));
    auto qItem = createQItem( HandlerMsgType::Inbound, 0UL, message);

    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);

    gethandleOfferUResource(g_testUURI);
    EXPECT_CALL(mockRouterInterface, routeInboundMsg(_)).Times(1);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    getProcessMessage(qItem);
}

/**
 *  @brief Unit test to verify the behavior of ProcessMessage for Stop type.
 */
TEST_F(SomeipHandlerClientTests, processMessageStopTest) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());

    auto stopItem = createQItem(HandlerMsgType::Stop, 0UL, nullptr);

    EXPECT_NO_THROW(getProcessMessage(stopItem));
}

/**
 *  @brief Unit test to verify the behavior of ProcessMessage for outbound type.
 */
TEST_F(SomeipHandlerClientTests, processMessageOutboundTest) {
    auto const type = UMessageType::UMESSAGE_TYPE_REQUEST;
    auto uuid = Uuidv8Factory::create();
    UAttributesBuilder builder(*g_testUURI,uuid, type, UPriority::UPRIORITY_CS4);
    builder.setSink(*g_testUURI);
    UAttributes uAttributes = builder.build();
        
    UMessage uMessage(g_payloadForHandler, uAttributes);
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(uMessage);
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);

    std::shared_ptr<vsomeip::message> message = createMessage();
    std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();

    gethandleOfferUResource(g_testUURI);

    setIsReadable(true);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockInterface, createRequest()).Times(1).WillOnce(Return(message));
    EXPECT_CALL(mockInterface, createPayload()).Times(1).WillOnce(Return(payload));
    EXPECT_CALL(mockSomeipInterface, send(_)).Times(1);

    auto qItem = createQItem(HandlerMsgType::Outbound, 0UL, uMsgPtr);
    getProcessMessage(qItem);
}

/**
 *  @brief Unit test to verify the behavior of ProcessMessage for Default type.
 */
TEST_F(SomeipHandlerClientTests, processMessageDefaultTest) {
    ON_CALL(mockRouterInterface, isStateRegistered())
        .WillByDefault(Return(true));

    ON_CALL(mockSomeipInterface, offerService(4660, 4369, '\0', 0))
        .WillByDefault(Return());

    auto defaultItem = createQItem(static_cast<HandlerMsgType>(999), 0UL, nullptr);

    EXPECT_NO_THROW(getProcessMessage(defaultItem));
}

/**
 *  @brief Unit test to verify the behavior of ProcessMessage for OfferUResource type.
 */
TEST_F(SomeipHandlerClientTests, processMessageOfferUResourceTest) {
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);

    auto OfferUResourceItem = createQItem(HandlerMsgType::OfferUResource, 0UL, g_testUURI);
    
    gethandleInboundSubscription(subStatusPtr);
    getProcessMessage(OfferUResourceItem);
    gethandleInboundSubscription(subStatusPtr);
    EXPECT_TRUE(getDoesInboundSubscriptionExist(this->eventGroup));
}

/**
 *  @brief Unit test to verify the behavior of ProcessMessage for InboundSubscriptionAck type
 */
TEST_F(SomeipHandlerClientTests, processMessageInboundSubscriptionAckTest) {
    UResourceId_t resourceId = 0x0102;
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);
    std::unique_ptr<UResource> resource = createUResource();
    std::shared_ptr<ResourceInformation> resourceInfo = std::make_shared<ResourceInformation>(*resource);
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);
    auto inboundSubscriptionAckItem = createQItem(HandlerMsgType::InboundSubscriptionAck, 0UL, subStatusPtr);

    getaddSubscriptionForRemoteService(resourceId, resourceInfo);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockRouterInterface, routeInboundMsg(_)).Times(1);
    getProcessMessage(inboundSubscriptionAckItem);
}

/**
 *  @brief Unit test to verify the behavior of ProcessMessage for InboundSubscription type
 */
TEST_F(SomeipHandlerClientTests, processMessageInboundSubscriptionTest) {
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);
    auto inboundSubscriptionItem = createQItem(HandlerMsgType::InboundSubscription, 0UL, subStatusPtr);
    gethandleOfferUResource(g_testUURI);

    getProcessMessage(inboundSubscriptionItem);
    EXPECT_TRUE(getDoesInboundSubscriptionExist(this->eventGroup));
}

/**
 *  @brief Verify that a payload is created from the UMessage.
 */
TEST_F(SomeipHandlerClientTests, buildSomeipPayloadFromUMessageTest) {
    const char* str = "100";
    const uint8_t* data = reinterpret_cast<const uint8_t*>(str);
    UPayload g_payloadForHandler(data, 4, UPayloadType::VALUE);
    auto uuid = Uuidv8Factory::create();
    UAttributesBuilder builderHandler(*g_testUURI, uuid, UMessageType::UMESSAGE_TYPE_PUBLISH, UPriority::UPRIORITY_CS2);
    UAttributes attributesHandler = builderHandler.build();

    UMessage message(g_payloadForHandler, attributesHandler);
    std::vector<uint8_t> result = getBuildSomeipPayloadFromUMessage(message);

    EXPECT_EQ(result.size(), 4);
}

/**
 *  @brief Test case for queueOfferUResource when handler is not running.
 */
TEST_F(SomeipHandlerClientTests, queueOfferUResourceRunningHandlerTest) {
    std::shared_ptr<UUri> uriPtr = g_testUURI;

    setRunning(true);
    handlerClient->queueOfferUResource(uriPtr);
    EXPECT_EQ(getQueueSize(), 1);
}

/**
 *  @brief Test to ensure a message is posted to the queue through onMessage if handler has a thread active.
 */
TEST_F(SomeipHandlerClientTests, onMessageTest) {
    std::shared_ptr<vsomeip::message> notification;
    notification = vsomeip::runtime::get()->create_request();
    notification->set_service(0x1234);
    notification->set_instance(0x1111);
    notification->set_method(0x3456);
    notification->set_message_type(message_type_e::MT_RESPONSE);

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > its_payload_data;
    for (vsomeip::byte_t i=0; i<10; i++) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    notification->set_payload(its_payload);

    setRunning(true);
    handlerClient->onMessage(notification);
    EXPECT_EQ(getQueueSize(), 1);
}

/**
 *  @brief Ensure quit clears the queue.
 */
TEST_F(SomeipHandlerClientTests, quitTest){
    std::shared_ptr<vsomeip::message> notification;
    notification = vsomeip::runtime::get()->create_request();
    notification->set_service(0x1234);
    notification->set_instance(0x1111);
    notification->set_method(0x3456);
    notification->set_message_type(message_type_e::MT_RESPONSE);

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > its_payload_data;
    for (vsomeip::byte_t i=0; i<10; i++) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    notification->set_payload(its_payload);

    setRunning(true);
    handlerClient->onMessage(notification);
    EXPECT_EQ(getQueueSize(), 1);

    getQuit();
    EXPECT_EQ(getQueueSize(), 0);
}

/**
 *  @brief Verify that handleSubscriptionRequestForRemoteService only acts on ResourceInformation objects when
 *  the router is in a registered state.
 */
TEST_F(SomeipHandlerClientTests, handleSubscriptionRequestForRemoteServiceRegisteredTest) {
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(g_messageHandler);

    EXPECT_CALL(mockRouterInterface, isStateRegistered()).WillOnce(Return(false));
    EXPECT_CALL(mockSomeipInterface, requestEvent(_, _, _, _, _, _)).Times(0);
    EXPECT_CALL(mockSomeipInterface, subscribe(_, _, _, _, _)).Times(0);
    getHandleSubscriptionRequestForRemoteService(uMsgPtr);

    EXPECT_CALL(mockRouterInterface, isStateRegistered()).WillOnce(Return(true));
    EXPECT_CALL(mockSomeipInterface, requestEvent(_, _, _, _, _, _)).Times(1);
    EXPECT_CALL(mockSomeipInterface, subscribe(_, _, _, _, _)).Times(1);
    getHandleSubscriptionRequestForRemoteService(uMsgPtr);
}

/**
 *  @brief Verify that nothing is done in HandleSubscriptionRequestForRemoteService if the subscription already exists.
 */
TEST_F(SomeipHandlerClientTests, handleSubscriptionRequestForRemoteServiceSubExistsTest) {
    UResourceId_t resourceId = 0x0123;
    std::unique_ptr<UResource> resource = createUResource();
    std::shared_ptr<ResourceInformation> resourceInfo = std::make_shared<ResourceInformation>(*resource);
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(g_messageHandler);
    
    EXPECT_CALL(mockSomeipInterface, requestEvent(_, _, _, _, _, _)).Times(1);
    EXPECT_CALL(mockSomeipInterface, subscribe(_, _, _, _, _)).Times(1);
    EXPECT_CALL(mockRouterInterface, isStateRegistered()).WillOnce(Return(true));
    getHandleSubscriptionRequestForRemoteService(uMsgPtr);

    std::ignore = getaddSubscriptionForRemoteService(resourceId, resourceInfo);
    EXPECT_CALL(mockRouterInterface, isStateRegistered()).WillOnce(Return(true));
    EXPECT_CALL(mockSomeipInterface, requestEvent(_, _, _, _, _, _)).Times(0);
    EXPECT_CALL(mockSomeipInterface, subscribe(_, _, _, _, _)).Times(0);
    getHandleSubscriptionRequestForRemoteService(uMsgPtr);
}

/**
 *  @brief Verify that handleSubscriptionRequestForRemoteService is used to route a subscription request.
 */
TEST_F(SomeipHandlerClientTests, handleOutboundRequestSubTest) {
    UMessage message(g_payloadForHandler, createUAttributes(UMESSAGE_TYPE_REQUEST));
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(message);

    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(0);
    getHandleOutboundMsg(uMsgPtr);
}

/**
 *  @brief Verify no notification is sent if the subscription does not exist.
 */
TEST_F(SomeipHandlerClientTests, handleOutboundNotificationNoSubTest) {
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(g_messageHandler);

    std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > payloadData;
    for (vsomeip::byte_t i = 0; i < 10; i++) {
        payloadData.push_back(i % 256);
    }
    payload->set_data(payloadData);

    EXPECT_CALL(mockSomeipInterface, createPayload()).Times(1).WillOnce(testing::Return(payload));
    EXPECT_CALL(mockSomeipInterface, notify(_, _, _, _, _)).Times(0);
    getHandleOutboundMsg(uMsgPtr);
}

/**
 *  @brief Verify that a notification is sent when the subscription exists.
 */
TEST_F(SomeipHandlerClientTests, handleOutboundNotificationSubTest) {
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(g_messageHandler);

    std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > payloadData;
    for (vsomeip::byte_t i = 0; i < 10; i++) {
        payloadData.push_back(i % 256);
    }
    payload->set_data(payloadData);

    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);

    gethandleOfferUResource(g_testUURI);
    gethandleInboundSubscription(subStatusPtr);
    EXPECT_CALL(mockSomeipInterface, createPayload()).Times(1).WillOnce(testing::Return(payload));
    EXPECT_CALL(mockSomeipInterface, notify(_, _, _, _, _)).Times(1);
    getHandleOutboundMsg(uMsgPtr);
}

/**
 *  @brief Verify that an inbound message is routed when the resource is present.
 */
TEST_F(SomeipHandlerClientTests, handleInboundRequestTest) {
    std::shared_ptr<vsomeip::message> message = createMessage();
    message->set_message_type(message_type_e::MT_REQUEST);
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);

    gethandleOfferUResource(g_testUURI);
    EXPECT_CALL(mockRouterInterface, routeInboundMsg(_)).Times(1);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    getHandleInboundMsg(message);
}

/**
 *  @brief Verify that an inbound response is only routed if the request ID exists in the map.
 */
TEST_F(SomeipHandlerClientTests, handleInboundResponseTest) {
    std::shared_ptr<vsomeip::message> message = createMessage();
    message->set_message_type(message_type_e::MT_RESPONSE);
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);
    request_t requestId= 0x1230456;
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(g_messageHandler);

    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(0);
    getHandleInboundMsg(message);

    addToSomeipReqIdToUTransportRequestLookup(requestId, uMsgPtr);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockRouterInterface, routeInboundMsg(_)).Times(1);
    getHandleInboundMsg(message);
}

/**
 *  @brief Ensure an ack is only routed when an even group exists for the ack.
 */
TEST_F(SomeipHandlerClientTests, handleInboundSubscriptionAckTest) {
    UResourceId_t resourceId = 0x0102;
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);
    std::unique_ptr<UResource> resource = createUResource();
    std::shared_ptr<ResourceInformation> resourceInfo = std::make_shared<ResourceInformation>(*resource);
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);

    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(0);
    getHandleInboundSubscriptionAck(subStatusPtr);
    

    getaddSubscriptionForRemoteService(resourceId, resourceInfo);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockRouterInterface, routeInboundMsg(_)).Times(1);
    getHandleInboundSubscriptionAck(subStatusPtr);
}

/**
 *  @brief Ensure an inbound notification is only routed if the subscription exists in the map.
 */
TEST_F(SomeipHandlerClientTests, handleInboundNotificationTest) {
    std::shared_ptr<vsomeip::message> message = createMessage();
    message->set_message_type(message_type_e::MT_NOTIFICATION);
    UResourceId_t resourceId = 0x0102;
    std::unique_ptr<UResource> resource = createUResource();
    std::shared_ptr<ResourceInformation> resourceInfo = std::make_shared<ResourceInformation>(*resource);
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);

    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(0).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockRouterInterface, routeInboundMsg(_)).Times(0);
    getHandleInboundMsg(message);

    getaddSubscriptionForRemoteService(resourceId, resourceInfo);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockRouterInterface, routeInboundMsg(_)).Times(1);
    getHandleInboundMsg(message);
}

/**
 *  @brief Verify subscriber is not added if the resource ID does not exist in the map.
 */
TEST_F(SomeipHandlerClientTests, handleInboundSubscriptionUnsupportedTest) {
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);

    gethandleInboundSubscription(subStatusPtr);
    EXPECT_FALSE(getDoesInboundSubscriptionExist(this->eventGroup));
}

/**
 *  @brief Verify that an unsubscribe request is processed correctly in handleInboundSubscription.
 */
TEST_F(SomeipHandlerClientTests, handleInboundSubscriptionUnsubscribeTest) {
    subscriptionStatus subStatus;
    subStatus.isSubscribed = true;
    subStatus.eventgroup = this->eventGroup;
    std::shared_ptr<subscriptionStatus> subStatusPtr = std::make_shared<subscriptionStatus>(subStatus);

    gethandleOfferUResource(g_testUURI);
    gethandleInboundSubscription(subStatusPtr);
    EXPECT_TRUE(getDoesInboundSubscriptionExist(this->eventGroup));

    subStatusPtr->isSubscribed = false;
    gethandleOfferUResource(g_testUURI);
    gethandleInboundSubscription(subStatusPtr);
    EXPECT_FALSE(getDoesInboundSubscriptionExist(this->eventGroup));
}

/**
 *  @brief Verify handleOutboundRequest only sends and adds a message to the map if the service is available.
 */
TEST_F(SomeipHandlerClientTests, handleOutboundRequestTest) {
    request_t requestId= 0x1230456;
    auto const type = UMessageType::UMESSAGE_TYPE_REQUEST;
    auto uuid = Uuidv8Factory::create();
    UAttributesBuilder builder(*g_testUURI,uuid, type, UPriority::UPRIORITY_CS4);
    builder.setSink(*g_testUURI);
    UAttributes uAttributes = builder.build();
        
    UMessage uMessage(g_payloadForHandler, uAttributes);
    std::shared_ptr<uprotocol::utransport::UMessage> uMsgPtr =
        std::make_shared<uprotocol::utransport::UMessage>(uMessage);
    MockSomeipInterface mockInterface;
    MessageTranslator translator(mockInterface);

    std::shared_ptr<vsomeip::message> message = createMessage();
    std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();

    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockInterface, createRequest()).Times(1).WillOnce(Return(message));
    EXPECT_CALL(mockInterface, createPayload()).Times(1).WillOnce(Return(payload));
    EXPECT_CALL(mockInterface, send(_)).Times(0);
    getHandleOutboundMsg(uMsgPtr);
    
    setIsReadable(true);
    EXPECT_CALL(mockRouterInterface, getMessageTranslator()).Times(1).WillOnce(testing::ReturnRef(translator));
    EXPECT_CALL(mockInterface, createRequest()).Times(1).WillOnce(Return(message));
    EXPECT_CALL(mockInterface, createPayload()).Times(1).WillOnce(Return(payload));
    EXPECT_CALL(mockSomeipInterface, send(_)).Times(1);
    getHandleOutboundMsg(uMsgPtr);
    EXPECT_TRUE(doesRequestExist(requestId));
}

/**
 *  @brief Ensure an outbound message is only posted to the queue if the thread is running.
 */
TEST_F(SomeipHandlerClientTests, queueOutboundMsgTest) {
    EXPECT_EQ(getQueueSize(), 0);
    handlerClient->queueOutboundMsg(g_messageHandler);

    setRunning(true);
    handlerClient->queueOutboundMsg(g_messageHandler);
    EXPECT_EQ(getQueueSize(), 1);
}

/**
 *  @brief Verify that the service availability is set to true only when isAvailable is true.
 */
TEST_F(SomeipHandlerClientTests, onAvailabilityTest) {
    bool isAvailable = true;

    EXPECT_FALSE(getIsReadable());

    handlerClient->onAvailability(this->service, g_instance, isAvailable);
    EXPECT_TRUE(getIsReadable());
}