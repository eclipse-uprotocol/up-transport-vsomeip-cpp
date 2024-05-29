#ifndef MOCK_SOME_IP_ROUTER_INTERFACE_HPP
#define MOCK_SOME_IP_ROUTER_INTERFACE_HPP

#include <gmock/gmock.h>
#include "SomeipRouterInterface.hpp"
#include "Logger.hpp"

using namespace vsomeip_v3;

class MockSomeipRouterInterface : public SomeipRouterInterface {
public:
    MOCK_METHOD(bool, routeInboundMsg, (uprotocol::utransport::UMessage &uMessage), (override));
    
    MOCK_METHOD(MessageTranslator&, getMessageTranslator, (), (override));
    
    MOCK_METHOD(bool, routeInboundSubscription, (std::string const &strTopic,
                                                 bool const &isSubscribe), (const, override));
    
    MOCK_METHOD(bool, isStateRegistered, (), (override));
};

#endif //MOCK_SOME_IP_ROUTER_INTERFACE_HPP
