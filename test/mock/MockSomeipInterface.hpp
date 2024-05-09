#include <gmock/gmock.h>
#include <string>
#include <set>
#include "SomeipInterface.hpp"
#include "Logger.hpp"
#include <chrono>

using namespace vsomeip_v3;

class MockSomeipInterface : public SomeipInterface {
public:
    MOCK_METHOD(const std::string&, getName, (), (const, override));
    
    MOCK_METHOD(client_t, getClient, (), (const, override));
    
    MOCK_METHOD(bool, init, (), (override));
    
    MOCK_METHOD(void, start, (), (override));
    
    MOCK_METHOD(void, stop, (), (override));
    
    MOCK_METHOD(void, offerService, (service_t, instance_t, major_version_t, minor_version_t), (override));
    
    MOCK_METHOD(void, stopOfferService, (service_t,
                                         instance_t,
                                         major_version_t,
                                         minor_version_t), (override));
    
    MOCK_METHOD(void, offerEvent, (service_t,
                                   instance_t,
                                   event_t,
                                   const std::set<eventgroup_t>&,
                                   event_type_e,
                                   std::chrono::milliseconds,
                                   bool,
                                   bool,
                                   const epsilon_change_func_t&,
                                   reliability_type_e), (override));

    MOCK_METHOD(void, requestService, (service_t, instance_t, major_version_t, minor_version_t), (override));

    MOCK_METHOD(void, releaseService, (service_t,
                                       instance_t), (override));

    MOCK_METHOD(void, requestEvent, (service_t,
                                     instance_t,
                                     event_t,
                                     const std::set<eventgroup_t>&,
                                     event_type_e,
                                     reliability_type_e), (override));

    MOCK_METHOD(void, releaseEvent, (service_t,
                                     instance_t,
                                     event_t), (override));

    MOCK_METHOD(void, subscribe, (service_t,
                                  instance_t,
                                  eventgroup_t,
                                  major_version_t,
                                  event_t), (override));

    MOCK_METHOD(void, unsubscribe, (service_t,
                                    instance_t,
                                    eventgroup_t), (override));

    MOCK_METHOD(bool, isAvailable, (service_t,
                                    instance_t,
                                    major_version_t,
                                    minor_version_t), (const, override));

    MOCK_METHOD(void, send, (std::shared_ptr<message>), (override));

    MOCK_METHOD(void, notify, (service_t,
                               instance_t,
                               event_t,
                               std::shared_ptr<payload>,
                               bool), (const, override));

    MOCK_METHOD(void, registerStateHandler, (const state_handler_t&), (override));

    MOCK_METHOD(void, unregisterStateHandler, (), (override));

    MOCK_METHOD(void, registerMessageHandler, (service_t,
                                               instance_t,
                                               method_t,
                                               const message_handler_t&), (override));

    MOCK_METHOD(void, unregisterMessageHandler, (service_t,
                                                 instance_t,
                                                 method_t), (override));

    MOCK_METHOD(void, registerAvailabilityHandler, (service_t,
                                                    instance_t,
                                                    const availability_handler_t&,
                                                    major_version_t,
                                                    minor_version_t), (override));

    MOCK_METHOD(void, unregisterAvailabilityHandler, (service_t,
                                                      instance_t,
                                                      major_version_t,
                                                      minor_version_t), (override));

    MOCK_METHOD(void, registerSubscriptionHandler, (service_t,
                                                    instance_t,
                                                    eventgroup_t,
                                                    const subscription_handler_sec_t&), (override));

    MOCK_METHOD(void, unregisterSubscriptionHandler, (service_t,
                                                      instance_t,
                                                      eventgroup_t), (override));

    MOCK_METHOD(void, clearAllHandler, (), (override));

    MOCK_METHOD(std::shared_ptr<message>, createRequest, (), (override));

    MOCK_METHOD(std::shared_ptr<message>, createResponse, (const std::shared_ptr<message>& request), (override));

    MOCK_METHOD(std::shared_ptr<payload>, createPayload, (), (override));

    MOCK_METHOD(void, registerSubscriptionStatusHandler, (service_t,
                                                          instance_t,
                                                          eventgroup_t,
                                                          event_t,
                                                          subscription_status_handler_t, bool), (override));

    MOCK_METHOD(void, unregisterSubscriptionStatusHandler, (service_t,
                                                            instance_t,
                                                            eventgroup_t,
                                                            event_t), (override));

    MOCK_METHOD(bool, isMethod, (const method_t &methodId), (const, override));

    MOCK_METHOD(std::shared_ptr<vsomeip::payload>, createPayload, (), (const));
};