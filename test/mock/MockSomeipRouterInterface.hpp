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
