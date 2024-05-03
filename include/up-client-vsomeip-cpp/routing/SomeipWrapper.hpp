/********************************************************************************
 * Copyright (c) 2024 Contributors to the Eclipse Foundation
 *
 * See the NOTICE file(s) distributed with this work for additional
 * information regarding copyright ownership.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 ********************************************************************************/

#ifndef VSOMEIP_WRAPPER_H
#define VSOMEIP_WRAPPER_H

#include "SomeipInterface.hpp"
#include <thread>

/**
 * @class
 * SomeipWrapper
 * @brief This class is a wrapper for the vsomeip library and implements the SomeipInterface
 *
 * @note Due to its heavy resource footprint, it should exist once per client.
 * It manages the lifecycle of the vsomeip client and
 * allocates all resources needed to communicate.
 */
class SomeipWrapper : public SomeipInterface {
public:
    /**
     * @brief @ref @see SomeipInterface::SomeipWrapper
     */
    SomeipWrapper() = default;

    /**
     * @brief @ref @see SomeipInterface::getName
     *
     * @return
     */
    std::string const& getName() const override;

    /**
     * @brief @ref SomeipInterface::getClient
     *
     * @return
     */
    vsomeip_v3::client_t getClient() const override;

    /**
     * @brief @see @ref SomeipInterface::eventLoopFn
     *
     * @note This method must be called after init to start message processing. It
     * will block until the message processing is terminated using the @ref
     * stop method or by receiving signals. It processes messages received
     * via the sockets and uses registered callbacks to pass them to the user
     * application.
     *
     * @param sipw
     *
     */
    static void eventLoopFn(SomeipWrapper * sipw);

    /**
     * @brief @see @ref SomeipInterface::init.
     */
    bool init() override;

    /**
     * @brief @see @ref SomeipInterface::start
     *
     */
    void start() override;

    /**
     * @brief @see @ref SomeipInterface::stop
     *
     */
    void stop() override;

    /**
     *
     * @brief @see @ref SomeipInterface::offerEvent
     * @note  @see @ref SomeipInterface::offerEvent
     * @param service @see @ref SomeipInterface::offerEvent
     * @param instance @see @ref SomeipInterface::offerEvent
     * @param eventID @see @ref SomeipInterface::offerEvent
     * @param eventgroups @see @ref SomeipInterface::offerEvent
     * @param type @see @ref SomeipInterface::offerEvent
     * @param cycle @see @ref SomeipInterface::offerEvent
     * @param change_resets_cycle @see @ref SomeipInterface::offerEvent
     * @param update_on_change @see @ref SomeipInterface::offerEvent
     * @param epsilon_change_func @see @ref SomeipInterface::offerEvent
     * @param reliability @see @ref SomeipInterface::offerEvent
     */
    void offerEvent(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::event_t eventID, std::set<vsomeip_v3::eventgroup_t> const &eventgroups,
        vsomeip_v3::event_type_e type = vsomeip_v3::event_type_e::ET_EVENT,
        std::chrono::milliseconds cycle = std::chrono::milliseconds::zero(),
        bool change_resets_cycle = false,
        bool update_on_change = true,
        vsomeip_v3::epsilon_change_func_t const &epsilon_change_func = nullptr,
        vsomeip_v3::reliability_type_e reliability = vsomeip_v3::reliability_type_e::RT_UNKNOWN) override;

    /**
     * @brief @see @ref SomeipInterface::offerService
     *
     * @param service
     * @param instance
     * @param majorVersion
     * @param minorVersion
     */
    void offerService(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::major_version_t majorVersion = vsomeip_v3::DEFAULT_MAJOR,
        vsomeip_v3::minor_version_t minorVersion = vsomeip_v3::DEFAULT_MINOR) override;

    /**
     * @brief @see @ref SomeipInterface::stopOfferService
     *
     * @param service
     * @param instance
     * @param majorVersion
     * @param minorVersion
     */
    void stopOfferService(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::major_version_t majorVersion = vsomeip_v3::DEFAULT_MAJOR,
        vsomeip_v3::minor_version_t minorVersion = vsomeip_v3::DEFAULT_MINOR) override;

    /**
     * @brief @see @ref SomeipInterface::requestService
     *
     * @param service
     * @param instance
     * @param majorVersion
     * @param minorVersion
     */
    void requestService(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::major_version_t majorVersion = vsomeip_v3::ANY_MAJOR,
        vsomeip_v3::minor_version_t minorVersion = vsomeip_v3::ANY_MINOR) override;

    /**
     *  @brief @see @ref SomeipInterface::releaseService
     *
     * @param service
     * @param instance
     */
    void releaseService(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance) override;

    /**
     * @brief @see @ref SomeipInterface::requestEvent
     *
     * @param service
     * @param instance
     * @param event
     * @param eventgroups
     * @param type
     * @param reliability
     */
    void requestEvent(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::event_t event,
        std::set<vsomeip_v3::eventgroup_t> const &eventgroups,
        vsomeip_v3::event_type_e type = vsomeip_v3::event_type_e::ET_EVENT,
        vsomeip_v3::reliability_type_e reliability = vsomeip_v3::reliability_type_e::RT_UNKNOWN) override;

    /**
    * @brief @see @ref SomeipInterface::releaseEvent
    *
    * @param service
    * @param instance
    * @param event
    */
    void releaseEvent(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::event_t event) override;

    /**
     * @brief @see @ref SomeipInterface::createRequest
     *
     * @return
     */
    std::shared_ptr<vsomeip_v3::message> createRequest() override;

    /**
     * @brief @see @ref SomeipInterface::createResponse
     *
     * @param request
     *
     * @return
     */
    std::shared_ptr<vsomeip_v3::message> createResponse(
        const std::shared_ptr<vsomeip_v3::message> &request) override;

    /**
     * @brief @see @ref SomeipInterface::createPayload
     *
     * @return
     */
    std::shared_ptr<vsomeip_v3::payload> createPayload() override;

    /**
     * @brief @see @ref SomeipInterface::subscribe
     *
     * @param service
     * @param instance
     * @param eventgroup
     * @param majorVersion
     * @param event
     */
    void subscribe(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::eventgroup_t eventgroup,
        vsomeip_v3::major_version_t majorVersion = vsomeip_v3::DEFAULT_MAJOR,
        vsomeip_v3::event_t event = vsomeip_v3::ANY_EVENT) override;

   /**
    * @brief @see @ref SomeipInterface::unSubscribe
    *
    * @param service
    * @param instance
    * @param eventgroup
    */
    void unsubscribe(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::eventgroup_t eventgroup) override;

   /**
    * @brief @see @ref SomeipInterface::isAvailable
    *
    * @param service
    * @param instance
    * @param majorVersion
    * @param minorVersion
    *
    * @return
    */
    bool isAvailable(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::major_version_t majorVersion = vsomeip_v3::ANY_MAJOR,
        vsomeip_v3::minor_version_t minorVersion = vsomeip_v3::ANY_MINOR) const override;

    /**
     * @brief @see @ref SomeipInterface::send
     *
     * @param message
     */
    void send(std::shared_ptr<vsomeip_v3::message> message) override;

    /**
     * @brief @see @ref SomeipInterface::notify
     *
     * @param service
     * @param instance
     * @param event
     * @param payload
     * @param force
    */
    void notify(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::event_t event,
        std::shared_ptr<vsomeip_v3::payload> payload,
        bool force = false) const override;

   /**
    * @brief @see @ref SomeipInterface::registerMessageHandler
    *
    * @param service
    * @param instance
    * @param method
    * @param handler
    */
    void registerMessageHandler(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::method_t method,
        vsomeip_v3::message_handler_t const &handler) override;

   /**
    * @brief @see @ref SomeipInterface::unregisterMessageHandler
    *
    * @param service
    * @param instance
    * @param method
    */
    void unregisterMessageHandler(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::method_t method) override;

    /**
     * @brief @see @ref SomeipInterface::registerAvailabilityHandler
     *
     * @param service
     * @param instance
     * @param handler
     * @param majorVersion
     * @param minorVersion
     */
    void registerAvailabilityHandler(vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::availability_handler_t const &handler,
        vsomeip_v3::major_version_t majorVersion = vsomeip_v3::ANY_MAJOR,
        vsomeip_v3::minor_version_t minorVersion = vsomeip_v3::ANY_MINOR) override;

   /**
    * @brief @see @ref SomeipInterface::unregisterAvailabilityHandler
    *
    * @param service
    * @param instance
    * @param majorVersion
    * @param minorVersion
    */
    void unregisterAvailabilityHandler(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::major_version_t majorVersion = vsomeip_v3::ANY_MAJOR,
        vsomeip_v3::minor_version_t minorVersion = vsomeip_v3::ANY_MINOR) override;

    /**
     * @brief @see @ref SomeipInterface::registerSubscriptionStatusHandler
     *
     * @param service
     * @param instance
     * @param eventgroup
     * @param event
     * @param handler
     * @param is_selective
     */
    void registerSubscriptionStatusHandler(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::eventgroup_t eventgroup,
        vsomeip_v3::event_t event,
        vsomeip_v3::subscription_status_handler_t handler,
        bool is_selective = false) override;

    /**
    * @brief @see @ref SomeipInterface::unregisterSubscriptionStatusHandler
    *
    * @param service
    * @param instance
    * @param eventgroup
    * @param event
    */
    void unregisterSubscriptionStatusHandler(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::eventgroup_t eventgroup,
        vsomeip_v3::event_t event) override;

    /**
    * @brief @see @ref SomeipInterface::registerSubscriptionHandler
    *
    * @param service
    * @param instance
    * @param eventgroup
    * @param handler
    */
    void registerSubscriptionHandler(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::eventgroup_t eventgroup,
        vsomeip_v3::subscription_handler_sec_t const &handler) override;

    /**
    * @brief @see @ref SomeipInterface::unregisterSubscriptionHandler
    *
    * @param service
    * @param instance
    * @param eventgroup
    */
    void unregisterSubscriptionHandler(
        vsomeip_v3::service_t service,
        vsomeip_v3::instance_t instance,
        vsomeip_v3::eventgroup_t eventgroup) override;

    /**
    * @brief @see @ref SomeipInterface::registerStateHandler
    *
    * @param handler
    */
    void registerStateHandler(vsomeip_v3::state_handler_t const &handler) override;

    /**
    * @brief @see @ref SomeipInterface::unregisterStateHandler
    */
    void unregisterStateHandler() override;

    /**
    * @brief @see @ref SomeipInterface::clearAllHandler
    */
    void clearAllHandler() override;

    /**
     * @brief @see @ref SomeipInterface::isMethod
     *
     * @param method
     * @return true
     * @return false
     */
    bool isMethod(const vsomeip_v3::method_t &methodId) const override;

private:
    /**
     * @brief Application of SomeipWrapper.
     *
     */
    std::shared_ptr<vsomeip::application> app_;

    /**
     * @brief Thread for Event Loop.
     *
     */
    std::thread someipEventLoopThread_;
}; // class SomeipWrapper

#endif /* VSOMEIP_WRAPPER_H */
