#include <gmock/gmock.h>
#include "SomeIPRouterInterface.hpp"
#include "Logger.h"

class MockSomeIPRouterInterface : public SomeIPRouterInterface {
public:
    MOCK_METHOD(bool, routeInboundMsg, (std::shared_ptr<UMsg>, SomeIPHandler&), (override));
    MOCK_METHOD(void, routeSubscriptionAck, (std::string const&, bool const&), ());
    MOCK_METHOD(MessageTranslator&, getMessageTranslator, (), (override));
    MOCK_METHOD(std::shared_ptr<UMsg>, getRequestForUuid, (const std::string&), (override));
    MOCK_METHOD(bool, isStateRegistered, (), (override));
    MOCK_METHOD(LocalDiscovery&, getLocalDiscovery, (), (override));
    void routeSubscriptionAck(const std::string& uuid, bool subscribed) override {}
    bool routeInboundSubscription(const std::string& strTopic, const bool& isSubscribe) const override {return true;}
};
