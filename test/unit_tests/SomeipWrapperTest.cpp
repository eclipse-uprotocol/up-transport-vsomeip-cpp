#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SomeipWrapper.hpp"
#include "mock/MockApplication.hpp"
#include "vsomeip/vsomeip.hpp"
#include "mock/CustomEventListener.hpp"

using namespace std::chrono_literals;
using ::testing::NiceMock;
using namespace vsomeip_v3;

/**
 *  @brief Service parameters.
 */
service_t const g_service = 0x1234U;
instance_t const g_instance = 0x1111U;
major_version_t const g_majorVersion = 2U;
minor_version_t const g_minorVersion = 1U;

/**
 *  @brief Handler parameter.
 */
method_t const g_method = 0x1111U;

/**
 *  @brief Event parameters.
 */
event_t const g_eventId = 0x1234U;
std::shared_ptr<std::set<eventgroup_t>> g_eventGroups = std::make_shared<std::set<eventgroup_t>>();
event_type_e const g_type = event_type_e::ET_EVENT;
std::chrono::milliseconds g_cycle = std::chrono::milliseconds::zero();
bool const g_changeResetCycles = false;
bool const g_updateOnChange = true;
epsilon_change_func_t const &g_epsilonChangeFunc = nullptr;
reliability_type_e const g_reliability = reliability_type_e::RT_UNKNOWN;

/**
 *  @brief Test fixture for SomepWrapper.
 */
class SomeIpWrapperTests : public testing::Test {
protected:
    /**
     *  @brief SomeipWrapper object for testing.
     */
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_;
    /**
     *  @brief SomeipWrapper mock app_ object for testing.
     */
    
    /**
     *  @brief Setup for SomeIpWrapper. Initializes required variables.
     */
    void SetUp() override {
        mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
        someIpWrapperInstance_.setApp(mockApp_);
    }
    
    /**
     *  @brief Teardown for SomeIpWrapper
     */
    void TearDown() override {
    }
};

/**
 *  @brief Constructor test.
 */
TEST_F(SomeIpWrapperTests, constructorTest) {
    EXPECT_NE(&someIpWrapperInstance_, nullptr);
}

/**
 *  @brief Test of the method getName which is a wrapper for SomeipInterface::get_name.
 */
TEST_F(SomeIpWrapperTests, getNameTest) {
    std::string wrapperName = CustomEventListener::getSomeipWrapperInstance().getName();

    EXPECT_FALSE(wrapperName.empty());
}

/**
 *  @brief Test of the method getClient which is a wrapper for SomeipInterface::get_client.
 */
TEST_F(SomeIpWrapperTests, getClientTest) {
    uint16_t wrapperClient = CustomEventListener::getSomeipWrapperInstance().getClient();

    EXPECT_NE(wrapperClient, 0);
}

/**
 *  @brief Test of the method start which is a wrapper for vsomeip::start(). The thread should only be joinable
 *  after start is called.
 */
TEST(SomeIpWrapperStandaloneTest, startTest) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_FALSE(someIpWrapperInstance_.getThread().joinable());

    someIpWrapperInstance_.start();
    EXPECT_TRUE(someIpWrapperInstance_.getThread().joinable());
    
    someIpWrapperInstance_.stop();
}


/**
 *  @brief Ensure start and stop work on the event loop properly.
 */
TEST(SomeIpWrapperStandaloneTest, StopTest) {
    SomeipWrapper someIpWrapperInstance_;
     std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);

    EXPECT_FALSE(someIpWrapperInstance_.getThread().joinable());

    someIpWrapperInstance_.start();
    EXPECT_TRUE(someIpWrapperInstance_.getThread().joinable());

    someIpWrapperInstance_.stop();
    EXPECT_FALSE(someIpWrapperInstance_.getThread().joinable());
}

/**
 *  @brief Test to ensure offerEvent calls vsomeip::offer_event.
 */
TEST(SomeIpWrapper, offerEventCall) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, offer_event(g_service,
                                       g_instance,
                                       g_eventId,
                                       *g_eventGroups,
                                       g_type,
                                       g_cycle,
                                       g_changeResetCycles,
                                       g_updateOnChange,
                                       ::testing::_,
                                       g_reliability)).Times(1);

    someIpWrapperInstance_.offerEvent(g_service,
                                      g_instance,
                                      g_eventId,
                                      *g_eventGroups,
                                      g_type,
                                      g_cycle,
                                      g_changeResetCycles,
                                      g_updateOnChange,
                                      g_epsilonChangeFunc,
                                      g_reliability);
}

/**
 *  @brief Test to ensure offerService calls vsomeip::offer_service.
 */
TEST(SomeIpWrapper, offerServiceCall) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, offer_service(g_service, g_instance, g_majorVersion, g_minorVersion)).Times(1);

    someIpWrapperInstance_.offerService(g_service, g_instance, g_majorVersion, g_minorVersion);
}

/**
 *  @brief Test to ensure offerService calls vsomeip::isAvailable.
 */
TEST(SomeIpWrapper, isAvailableCall) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, is_available(g_service, g_instance, g_majorVersion, g_minorVersion)).Times(1);

    someIpWrapperInstance_.isAvailable(g_service, g_instance, g_majorVersion, g_minorVersion);
}

/**
 *  @brief Test to ensure stopOfferService calls vsomeip::stop_offer_service.
 */
TEST(SomeIpWrapper, stopOfferServiceCall) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, stop_offer_service(g_service, g_instance, g_majorVersion, g_minorVersion)).Times(1);

    someIpWrapperInstance_.stopOfferService(g_service, g_instance, g_majorVersion, g_minorVersion);
}


/**
 *  @brief Test to ensure requestService calls vsomeip::request_service.
 */
TEST(SomeIpWrapper, requestServiceCall) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, request_service(g_service, g_instance, g_majorVersion, g_minorVersion)).Times(1);

    someIpWrapperInstance_.requestService(g_service, g_instance, g_majorVersion, g_minorVersion);
}

/**
 *  @brief Test to ensure releaseService calls vsomeip::release_service.
 */
TEST(SomeIpWrapper, releaseServiceCall) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, release_service(g_service, g_instance)).Times(1);

    someIpWrapperInstance_.releaseService(g_service, g_instance);
}

/**
 *  @brief Test to ensure requestEvent calls vsomeip::request_event.
 */
TEST(SomeIpWrapper, requestEventCall) {
    SomeipWrapper someIpWrapperInstance_;
    std::shared_ptr<::testing::NiceMock<MockApplication>> mockApp_ = std::make_shared<::testing::NiceMock<MockApplication>>();
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, request_event(g_service,
                                         g_instance,
                                         g_eventId,
                                         *g_eventGroups,
                                         g_type,
                                         g_reliability)).Times(1);

    someIpWrapperInstance_.requestEvent(g_service, g_instance, g_eventId, *g_eventGroups, g_type, g_reliability);
}

/**
 *  @brief Test to ensure releaseEvent calls vsomeip::release_event.
 */
TEST_F(SomeIpWrapperTests, releaseEventCall) {
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, release_event(g_service, g_instance, g_eventId)).Times(1);

    someIpWrapperInstance_.releaseEvent(g_service, g_instance, g_eventId);
}

/**
 *  @brief Test to ensure createRequest does not return a nullptr.
 */
TEST_F(SomeIpWrapperTests, createRequestTest) {
    std::shared_ptr<message> request;
    request = someIpWrapperInstance_.createRequest();

    EXPECT_NE(request, nullptr);
}

/**
 *  @brief Test to ensure createResponse returns a response and not nullptr when the request parameter is not null.
 */
TEST_F(SomeIpWrapperTests, createResponseTest) {
    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(0x1234);
    request->set_instance(0x1111);
    request->set_method(0x3456);
 
    std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > payloadData;
    for (vsomeip::byte_t i = 0; i < 10; i++) {
        payloadData.push_back(i % 256);
    }
    payload->set_data(payloadData);
    request->set_payload(payload);
    std::shared_ptr<message> response;
    response = someIpWrapperInstance_.createResponse(request);

    EXPECT_NE(response, nullptr);
}

/**
 *  @brief Test to ensure createResponse can handle a nullptr request parameter.
 */
TEST_F(SomeIpWrapperTests, createResponseNullTest) {
    std::shared_ptr<vsomeip::message> request = nullptr;
    std::shared_ptr<message> response;
    response = someIpWrapperInstance_.createResponse(request);

    EXPECT_EQ(nullptr, response);
}

/**
 *  @brief Test to ensure createPayload does not return a nullptr.
 */
TEST_F(SomeIpWrapperTests, createPayloadTest) {
    std::shared_ptr<payload> payload;
    payload = someIpWrapperInstance_.createPayload();

    EXPECT_NE(payload, nullptr);
}

/**
 *  @brief Test to ensure subscribe calls vsomeip::subscribe.
 */
TEST_F(SomeIpWrapperTests, subscribeCall) {
    eventgroup_t eventGroup = 0x1111U;

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, subscribe(g_service, g_instance, eventGroup, g_majorVersion, g_eventId)).Times(1);

    someIpWrapperInstance_.subscribe(g_service, g_instance, eventGroup, g_majorVersion, g_eventId);
}

/**
 *  @brief Test to ensure unsubscribe calls vsomeip::unsubscribe.
 */
TEST_F(SomeIpWrapperTests, unsubscribeCall) {
    eventgroup_t eventGroup = 0x1111U;

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, unsubscribe(g_service, g_instance, eventGroup)).Times(1);

    someIpWrapperInstance_.unsubscribe(g_service, g_instance, eventGroup);
}

/**
 *  @brief Test to ensure isAvailable calls vsomeip::is_available.
 */
TEST_F(SomeIpWrapperTests, isAvailableCall) {
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, is_available(g_service, g_instance, g_majorVersion, g_minorVersion)).Times(1);

    std::ignore = someIpWrapperInstance_.isAvailable(g_service, g_instance, g_majorVersion, g_minorVersion);
}

/**
 *  @brief Test to ensure send calls vsomeip::send.
 */
TEST_F(SomeIpWrapperTests, sendCall) {
    someIpWrapperInstance_.setApp(mockApp_);

    std::shared_ptr<vsomeip::message> message;
    message = vsomeip::runtime::get()->create_request();
    message->set_service(0x1234);
    message->set_instance(0x1111);
    message->set_method(0x3456);
 
    std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get()->create_payload();
    std::vector< vsomeip::byte_t > payloadData;
    for (vsomeip::byte_t i = 0; i < 10; i++) {
        payloadData.push_back(i % 256);
    }
    payload->set_data(payloadData);
    message->set_payload(payload);

    EXPECT_CALL(*mockApp_, send(message)).Times(1);

    someIpWrapperInstance_.send(message);
}

/**
 *  @brief Test to ensure an empty message can't be sent.
 */
TEST_F(SomeIpWrapperTests, sendEmptyMessageCall) {
    std::shared_ptr<vsomeip::message> message;

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, send(message)).Times(0);

    someIpWrapperInstance_.send(message);
}

/**
 *  @brief Test to ensure notify calls vsomeip::notify.
 */
TEST_F(SomeIpWrapperTests, notifyCall) {
    std::shared_ptr<payload> payload;
    payload = someIpWrapperInstance_.createPayload();

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, notify(g_service, g_instance, g_eventId, payload, true)).Times(1);

    someIpWrapperInstance_.notify(g_service, g_instance, g_eventId, payload, true);
}


/**
 *  @brief Test to ensure registerMessageHandler calls vsomeip::register_message_handler.
 */
TEST_F(SomeIpWrapperTests, registerMessageHandlerCall) {
    message_handler_t handler;

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, register_message_handler(g_service, g_instance, g_method, ::testing::_)).Times(1);

    someIpWrapperInstance_.registerMessageHandler(g_service, g_instance, g_method, handler);
}

/**
 *  @brief Test to ensure unregisterMessageHandler calls vsomeip::unregister_message_handler.
 */
TEST_F(SomeIpWrapperTests, unregisterMessageHandlerCall) {
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, unregister_message_handler(g_service, g_instance, g_method)).Times(1);

    someIpWrapperInstance_.unregisterMessageHandler(g_service, g_instance, g_method);
}

/**
 *  @brief Test to ensure registerAvailabilityHandler calls vsomeip::register_availability_handler.
 */
TEST_F(SomeIpWrapperTests, registerAvailabilityHandlerCall) {
    availability_handler_t handler = [](service_t service, instance_t instance, bool available){    (void)service;
                                                                                                    (void)instance;
                                                                                                    (void)available;};

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, register_availability_handler(g_service,
                                                         g_instance,
                                                         ::testing::A<availability_handler_t const&>(),
                                                         g_majorVersion,
                                                         g_minorVersion)).Times(1);

    someIpWrapperInstance_.registerAvailabilityHandler(g_service, g_instance, handler, g_majorVersion, g_minorVersion);
}

/**
 *  @brief Test to ensure unregisterAvailabilityHandler calls vsomeip::unregister_availability_handler.
 */
TEST_F(SomeIpWrapperTests, unregisterAvailabilityHandlerCall) {
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, unregister_availability_handler(g_service,
                                                           g_instance,
                                                           g_majorVersion,
                                                           g_minorVersion)).Times(1);

    someIpWrapperInstance_.unregisterAvailabilityHandler(g_service, g_instance, g_majorVersion, g_minorVersion);
}

/**
 *  @brief Test to ensure registerSubscriptionStatusHandler calls vsomeip::register_subscription_status_handler.
 */
TEST_F(SomeIpWrapperTests, registerSubscriptionStatusHandlerCall) {
    subscription_status_handler_t handler = [](service_t const service,
                                               instance_t const instance,
                                               eventgroup_t const eventgroup,
                                               event_t const event,
                                               uint16_t const status) {     (void)service;
                                                                            (void)instance;
                                                                            (void)eventgroup;
                                                                            (void)event;
                                                                            (void)status;};

    bool const isSelective = false;
    eventgroup_t const eventGroup = 0x1111U;

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, register_subscription_status_handler(g_service,
                                                                g_instance,
                                                                eventGroup,
                                                                g_eventId,
                                                                ::testing::_,
                                                                isSelective)).Times(1);

    someIpWrapperInstance_.registerSubscriptionStatusHandler(g_service,
                                                             g_instance,
                                                             eventGroup,
                                                             g_eventId,
                                                             handler,
                                                             isSelective);
}

/**
 *  @brief Test to ensure unregisterSubscriptionStatusHandler calls vsomeip::unregister_subscription_status_handler.
 */
TEST_F(SomeIpWrapperTests, unregisterSubscriptionStatusHandlerCall) {
    eventgroup_t const eventGroup = 0x1111U;

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, unregister_subscription_status_handler(g_service,
                                                                  g_instance,
                                                                  eventGroup,
                                                                  g_eventId)).Times(1);

    someIpWrapperInstance_.unregisterSubscriptionStatusHandler(g_service, g_instance, eventGroup, g_eventId);
}

/**
 *  @brief Test to ensure registerSubscriptionHandler calls vsomeip::register_subscription_handler.
 */
TEST_F(SomeIpWrapperTests, registerSubscriptionHandlerCall) {
    subscription_handler_sec_t handler = [](client_t client,
                                            vsomeip_sec_client_t const* sec_client,
                                            std::string const& str,
                                            bool b) -> bool {   (void)client;
                                                                (void)sec_client;
                                                                (void)str;
                                                                (void)b;
                                                                return true;
                                                            };

    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, register_subscription_handler(g_service,
                                                         g_instance,
                                                         g_eventId,
                                                         ::testing::A<subscription_handler_sec_t const&>())).Times(1);

    someIpWrapperInstance_.registerSubscriptionHandler(g_service, g_instance, g_eventId, handler);
}

/**
 *  @brief Test to ensure unregisterSubscriptionHandler calls vsomeip::unregister_subscription_handler.
 */
TEST_F(SomeIpWrapperTests, unregisterSubscriptionHandlerCall) {
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, unregister_subscription_handler(g_service, g_instance, g_eventId)).Times(1);

    someIpWrapperInstance_.unregisterSubscriptionHandler(g_service, g_instance, g_eventId);
}

/**
 *  @brief Test to ensure registerStateHandler calls vsomeip::register_state_handler.
 */
TEST_F(SomeIpWrapperTests, registerStateHandlerCall) {
    state_handler_t handler = [](state_type_e state) {(void)state;};
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, register_state_handler(::testing::_)).Times(1);

    someIpWrapperInstance_.registerStateHandler(handler);
}

/**
 *  @brief Test to ensure unregisterStateHandler calls vsomeip::unregister_state_handler.
 */
TEST_F(SomeIpWrapperTests, unregisterStateHandlerCall) {
    state_handler_t handler = [](state_type_e state) {(void)state;};
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, unregister_state_handler()).Times(1);

    someIpWrapperInstance_.unregisterStateHandler();
}

/**
 *  @brief Test to ensure clearAllHandler calls vsomeip::clear_all_handler.
 */
TEST_F(SomeIpWrapperTests, clearAllHandlerCall) {
    someIpWrapperInstance_.setApp(mockApp_);
    EXPECT_CALL(*mockApp_, clear_all_handler()).Times(1);

    someIpWrapperInstance_.clearAllHandler();
}

/**
 *  @brief Test to ensure isMethod correctly determines if a method ID is in range.
 */
TEST_F(SomeIpWrapperTests, isMethodRangeTest) {
    uint16_t const eventIdRangeStart = 0x8000;
    uint16_t const eventIdRangeEnd = 0x8FFF;

    EXPECT_FALSE(someIpWrapperInstance_.isMethod(eventIdRangeStart));
    EXPECT_FALSE(someIpWrapperInstance_.isMethod(eventIdRangeEnd));
    EXPECT_FALSE(someIpWrapperInstance_.isMethod((eventIdRangeStart + eventIdRangeEnd) / 2));
    EXPECT_TRUE(someIpWrapperInstance_.isMethod(eventIdRangeStart - 1));
    EXPECT_TRUE(someIpWrapperInstance_.isMethod(eventIdRangeEnd + 1));
}