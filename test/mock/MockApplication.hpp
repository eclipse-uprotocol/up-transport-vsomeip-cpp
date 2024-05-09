#include <gmock/gmock.h>
#include "SomeipInterface.hpp"
#include <vsomeip/application.hpp>

using namespace vsomeip_v3;

class MockApplication : public vsomeip::application {
public:
    MockApplication();
    virtual ~MockApplication();
    
    MOCK_METHOD(void, start, (), (override));
    
    MOCK_METHOD(void, stop, (), (override));
    
    MOCK_METHOD(const std::string&, get_name, (), (const, override));
    
    MOCK_METHOD(client_t, get_client, (), (const, override));
    
    MOCK_METHOD(diagnosis_t, get_diagnosis, (), (const, override));
    
    MOCK_METHOD(bool, init, (), (override));
    
    MOCK_METHOD(void, process, (int _number), (override));
    
    MOCK_METHOD(security_mode_e, get_security_mode, (), (const, override));
    
    MOCK_METHOD(void, offer_service, (service_t,
                                      instance_t,
                                      major_version_t,
                                      minor_version_t), (override));
    
    MOCK_METHOD(void, stop_offer_service, (service_t,
                                           instance_t,
                                           major_version_t,
                                           minor_version_t), (override));
    
    MOCK_METHOD(void, offer_event, (service_t,
                                    instance_t,
                                    event_t,
                                    const std::set<eventgroup_t>&,
                                    event_type_e,
                                    std::chrono::milliseconds,
                                    bool,
                                    bool,
                                    const epsilon_change_func_t&,
                                    reliability_type_e), (override));
    
    MOCK_METHOD(void, stop_offer_event, (service_t,
                                         instance_t,
                                         event_t), (override));
    
    MOCK_METHOD(void, request_service, (service_t,
                                        instance_t,
                                        major_version_t,
                                        minor_version_t), (override));
    
    MOCK_METHOD(void, release_service, (service_t,
                                        instance_t), (override));
    
    MOCK_METHOD(void, request_event, (service_t,
                                      instance_t,
                                      event_t,
                                      const std::set<eventgroup_t>&,
                                      event_type_e,
                                      reliability_type_e), (override));
    
    MOCK_METHOD(void, release_event, (service_t,
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
    
    MOCK_METHOD(bool, is_available, (service_t,
                                     instance_t,
                                     major_version_t,
                                     minor_version_t), (const, override));
    
    MOCK_METHOD(bool, are_available, (application::available_t&,
                                      service_t,
                                      instance_t,
                                      major_version_t,
                                      minor_version_t), (const, override));
    
    MOCK_METHOD(void, send, (std::shared_ptr<message>), (override));
    
    MOCK_METHOD(void, notify, (service_t,
                               instance_t,
                               event_t,
                               std::shared_ptr<payload>,
                               bool), (const, override));
    
    MOCK_METHOD(void, notify_one, (service_t,
                                   instance_t,
                                   event_t,
                                   std::shared_ptr<payload>,
                                   client_t,
                                   bool), (const, override));
    
    MOCK_METHOD(void, register_state_handler, (const state_handler_t&), (override));
    
    MOCK_METHOD(void, unregister_state_handler, (), (override));
    
    MOCK_METHOD(void, register_message_handler, (service_t,
                                                 instance_t,
                                                 method_t,
                                                 const message_handler_t&), (override));
    
    MOCK_METHOD(void, unregister_message_handler, (service_t,
                                                   instance_t,
                                                   method_t), (override));
    
    MOCK_METHOD(void, register_availability_handler, (service_t,
                                                      instance_t,
                                                      const availability_handler_t&,
                                                      major_version_t,
                                                      minor_version_t), (override));
    
    MOCK_METHOD(void, unregister_availability_handler, (service_t,
                                                        instance_t,
                                                        major_version_t,
                                                        minor_version_t), (override));
    
    MOCK_METHOD(void, register_subscription_handler, (service_t,
                                                      instance_t,
                                                      eventgroup_t,
                                                      const subscription_handler_t&), (override));
    
    MOCK_METHOD(void, register_async_subscription_handler, (service_t,
                                                            instance_t,
                                                            eventgroup_t,
                                                            const async_subscription_handler_t&), (override));
    
    MOCK_METHOD(void, unregister_subscription_handler, (service_t,
                                                        instance_t,
                                                        eventgroup_t), (override));
    
    MOCK_METHOD(void, clear_all_handler, (), (override));
    
    MOCK_METHOD(bool, is_routing, (), (const, override));
    
    MOCK_METHOD(void, set_routing_state, (routing_state_e), (override));
    
    MOCK_METHOD(void, unsubscribe, (service_t,
                                    instance_t,
                                    eventgroup_t,
                                    event_t), (override));
    
    MOCK_METHOD(void, register_subscription_status_handler, (service_t,
                                                             instance_t,
                                                             eventgroup_t,
                                                             event_t,
                                                             subscription_status_handler_t,
                                                             bool), (override));
    
    MOCK_METHOD(void, unregister_subscription_status_handler, (service_t,
                                                               instance_t,
                                                               eventgroup_t,
                                                               event_t), (override));
    
    MOCK_METHOD(void, get_offered_services_async, (offer_type_e,
                                                   const offered_services_handler_t&), (override));
    
    MOCK_METHOD(void, set_watchdog_handler, (const watchdog_handler_t&,
                                             std::chrono::seconds), (override));
    
    MOCK_METHOD(void, set_sd_acceptance_required, (const remote_info_t&,
                                                   const std::string&,
                                                   bool), (override));
    
    MOCK_METHOD(void, set_sd_acceptance_required, (const sd_acceptance_map_type_t&,
                                                   bool), (override));
    
    MOCK_METHOD(sd_acceptance_map_type_t, get_sd_acceptance_required, (), (override));
    
    MOCK_METHOD(void, register_sd_acceptance_handler, (const sd_acceptance_handler_t&), (override));
    
    MOCK_METHOD(void, register_reboot_notification_handler, (const reboot_notification_handler_t&), (override));
    
    MOCK_METHOD(void, register_routing_ready_handler, (const routing_ready_handler_t&), (override));
    
    MOCK_METHOD(void, register_routing_state_handler, (const routing_state_handler_t&), (override));
    
    MOCK_METHOD(bool, update_service_configuration, (service_t,
                                                     instance_t,
                                                     uint16_t,
                                                     bool,
                                                     bool,
                                                     bool), (override));
    
    MOCK_METHOD(void, update_security_policy_configuration, (uint32_t,
                                                             uint32_t,
                                                             std::shared_ptr<policy>,
                                                             std::shared_ptr<payload>,
                                                             const security_update_handler_t&), (override));
    
    MOCK_METHOD(void, remove_security_policy_configuration, (uint32_t,
                                                             uint32_t,
                                                             const security_update_handler_t&), (override));
    
    MOCK_METHOD(void, register_subscription_handler, (service_t,
                                                      instance_t,
                                                      eventgroup_t,
                                                      const subscription_handler_ext_t&), (override));
    
    MOCK_METHOD(void, register_async_subscription_handler, (service_t,
                                                            instance_t,
                                                            eventgroup_t,
                                                            const async_subscription_handler_ext_t&), (override));
    
    MOCK_METHOD(void, subscribe_with_debounce, (service_t,
                                                instance_t,
                                                eventgroup_t,
                                                major_version_t,
                                                event_t,
                                                const debounce_filter_t&), (override));
    
    MOCK_METHOD(void, register_message_acceptance_handler, (const message_acceptance_handler_t&), (override));
    
    MOCK_METHOD((std::map<std::string, std::string>), get_additional_data, (const std::string&), (override));
    
    MOCK_METHOD(void, register_availability_handler, (service_t,
                                                      instance_t,
                                                      const availability_state_handler_t&,
                                                      major_version_t,
                                                      minor_version_t), (override));
    
    MOCK_METHOD(void, register_subscription_handler, (service_t,
                                                      instance_t,
                                                      eventgroup_t,
                                                      const subscription_handler_sec_t&), (override));
    
    MOCK_METHOD(void, register_async_subscription_handler, (service_t,
                                                            instance_t,
                                                            eventgroup_t,
                                                            async_subscription_handler_sec_t), (override));
    
    MOCK_METHOD(void, register_message_handler_ext, (service_t,
                                                     instance_t,
                                                     method_t,
                                                     const message_handler_t&,
                                                     handler_registration_type_e), (override));
    };