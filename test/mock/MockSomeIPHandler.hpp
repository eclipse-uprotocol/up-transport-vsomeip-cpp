#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>
#include "Logger.h"
#include "SomeIPHandler.hpp"

class MockSomeIPHandler {
public:
    MOCK_METHOD(bool, addThread, ());
    MOCK_METHOD(void, removeThread, ());
    MOCK_METHOD(void, executor, ());
    MOCK_METHOD(void, flush, ());
    MOCK_METHOD(void, incrementInboundSubscriptionCount, (vsomeip_v3::eventgroup_t eventGroup));
    MOCK_METHOD(void, decrementInboundSubscriptionCount, (vsomeip_v3::eventgroup_t eventGroup));
    MOCK_METHOD(std::string, buildTopicKey, (vsomeip_v3::service_t const &service_id, vsomeip_v3::eventgroup_t const &eventID), (const));
    MOCK_METHOD(void, handleInboundMsg, (std::shared_ptr<message> const msg));
    MOCK_METHOD(void, handleInboundSubscriptionAck, (std::shared_ptr<subscriptionStatus> const subStatus));
    MOCK_METHOD(void, handleInboundNotification, (std::shared_ptr<message> sMsg));
    MOCK_METHOD(void, handleInboundRequest, (std::shared_ptr<message> sMsg));
    MOCK_METHOD(void, handleInboundResponse, (std::shared_ptr<message> sMsg));
    MOCK_METHOD(void, handleInboundSubscription, (std::shared_ptr<subscriptionStatus> const subStatus));
    MOCK_METHOD(void, handleOutboundNotification, (std::shared_ptr<UMsg> const uMsg), (const));
    MOCK_METHOD(void, handleOutboundRequest, (std::shared_ptr<UMsg> uMsg));
    MOCK_METHOD(void, handleOutboundResponse, (std::shared_ptr<UMsg> uMsg));
    MOCK_METHOD(void, handleOutboundSubscription, (std::shared_ptr<UMsg> const uMsg));
    MOCK_METHOD(void, registerSubscriptionStatusHandler, (vsomeip_v3::eventgroup_t const eventGroup));
    MOCK_METHOD(void, registerMessageHandler, ());
    MOCK_METHOD(bool, doesSubscriptionExist, (vsomeip_v3::eventgroup_t const eventGroup));
    MOCK_METHOD(void, simulateSubscriptionAck, (vsomeip_v3::eventgroup_t const eventGroup));
    MOCK_METHOD(void, addSubscriptionEntry, (vsomeip_v3::eventgroup_t eventGroup, std::string const& uuid, std::string const& uPTopic));
    MOCK_METHOD(void, addNewUuidToSubscriptionMap, (vsomeip_v3::eventgroup_t eventGroup, std::string& uuid, std::string& uPTopic));
    MOCK_METHOD(void, handleOutboundMsg, (std::shared_ptr<UMsg>), ());

};

