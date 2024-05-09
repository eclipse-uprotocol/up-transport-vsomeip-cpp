#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>
#include "Logger.hpp"
#include "SomeipHandler.hpp"

using namespace vsomeip_v3;
 
class MockSomeipHandler : public SomeipHandler {
public:

    MockSomeipHandler(
        SomeipInterface &someIpLibInterface,
        SomeipRouterInterface &someIpRouterLibInterface,
        HandlerType const someIpRouterHandlerType,
        UEntity const &uEntityInfo,
        UAuthority const &uAuthorityInfo,
        instance_t const instanceIdentifier,
        uint16_t qPriorityLevels
    ) : SomeipHandler(
            someIpLibInterface,
            someIpRouterLibInterface,
            someIpRouterHandlerType,
            uEntityInfo,
            uAuthorityInfo,
            instanceIdentifier,
            qPriorityLevels
        ) {}

    MOCK_METHOD(bool, addThread, ());
 
    MOCK_METHOD(void, removeThread, ());
    
    MOCK_METHOD(void, executor, ());
    
    MOCK_METHOD(void, flush, ());
    
    MOCK_METHOD(void, incrementInboundSubscriptionCount, (eventgroup_t eventGroup));
    
    MOCK_METHOD(void, decrementInboundSubscriptionCount, (eventgroup_t eventGroup));
    
    MOCK_METHOD(std::string, buildTopicKey, (service_t const &service_id, eventgroup_t const &eventID), (const));
    
    MOCK_METHOD(void, handleInboundMsg, (std::shared_ptr<message> const msg));
    
    MOCK_METHOD(void, handleInboundSubscriptionAck, (std::shared_ptr<subscriptionStatus> const subStatus));
    
    MOCK_METHOD(void, handleInboundNotification, (std::shared_ptr<message> sMsg));
    
    MOCK_METHOD(void, handleInboundRequest, (std::shared_ptr<message> sMsg));
    
    MOCK_METHOD(void, handleInboundResponse, (std::shared_ptr<message> sMsg));
    
    MOCK_METHOD(void, handleInboundSubscription, (std::shared_ptr<subscriptionStatus> const subStatus));
    
    MOCK_METHOD(void,
                handleOutboundNotification,
                (std::shared_ptr<uprotocol::utransport::UMessage> const uMsg), (const));
    
    MOCK_METHOD(void, handleOutboundRequest, (std::shared_ptr<uprotocol::utransport::UMessage> uMsg));
    
    MOCK_METHOD(void, handleOutboundResponse, (std::shared_ptr<uprotocol::utransport::UMessage> uMsg));
    
    MOCK_METHOD(void, handleOutboundSubscription, (std::shared_ptr<uprotocol::utransport::UMessage> const uMsg));
    
    MOCK_METHOD(void, registerSubscriptionStatusHandler, (eventgroup_t const eventGroup));
    
    MOCK_METHOD(void, registerMessageHandler, ());
    
    MOCK_METHOD(bool, doesSubscriptionExist, (eventgroup_t const eventGroup));
    
    MOCK_METHOD(void, simulateSubscriptionAck, (eventgroup_t const eventGroup));
    
    MOCK_METHOD(void, addSubscriptionEntry, (eventgroup_t eventGroup,
                                             std::string const& uuid,
                                             std::string const& uPTopic));
    
    MOCK_METHOD(void, addNewUuidToSubscriptionMap, (eventgroup_t eventGroup, std::string& uuid, std::string& uPTopic));
    
    MOCK_METHOD(void, handleOutboundMsg, (std::shared_ptr<uprotocol::utransport::UMessage>), ());
};