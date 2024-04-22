/*
 * Copyright (c) 2024 General Motors GTO LLC
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-FileType: SOURCE
 * SPDX-FileCopyrightText: 2024 General Motors GTO LLC
 * SPDX-License-Identifier: Apache-2.0
 */

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
    client_t getClient() const override;

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
        service_t service,
        instance_t instance,
        event_t eventID, std::set<eventgroup_t> const &eventgroups,
        event_type_e type = event_type_e::ET_EVENT,
        std::chrono::milliseconds cycle = std::chrono::milliseconds::zero(),
        bool change_resets_cycle = false,
        bool update_on_change = true,
        epsilon_change_func_t const &epsilon_change_func = nullptr,
        reliability_type_e reliability = reliability_type_e::RT_UNKNOWN) override;

    /**
     * @brief @see @ref SomeipInterface::offerService
     *
     * @param service
     * @param instance
     * @param majorVersion
     * @param minorVersion
     */
    void offerService(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = DEFAULT_MAJOR,
        minor_version_t minorVersion = DEFAULT_MINOR) override;

    /**
     * @brief @see @ref SomeipInterface::stopOfferService
     *
     * @param service
     * @param instance
     * @param majorVersion
     * @param minorVersion
     */
    void stopOfferService(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = DEFAULT_MAJOR,
        minor_version_t minorVersion = DEFAULT_MINOR) override;

    /**
     * @brief @see @ref SomeipInterface::requestService
     *
     * @param service
     * @param instance
     * @param majorVersion
     * @param minorVersion
     */
    void requestService(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) override;

    /**
     *  @brief @see @ref SomeipInterface::releaseService
     *
     * @param service
     * @param instance
     */
    void releaseService(
        service_t service,
        instance_t instance) override;

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
        service_t service,
        instance_t instance,
        event_t event,
        std::set<eventgroup_t> const &eventgroups,
        event_type_e type = event_type_e::ET_EVENT,
        reliability_type_e reliability = reliability_type_e::RT_UNKNOWN) override;

    /**
    * @brief @see @ref SomeipInterface::releaseEvent
    *
    * @param service
    * @param instance
    * @param event
    */
    void releaseEvent(
        service_t service,
        instance_t instance,
        event_t event) override;

    /**
     * @brief @see @ref SomeipInterface::createRequest
     *
     * @return
     */
    std::shared_ptr<message> createRequest() override;

    /**
     * @brief @see @ref SomeipInterface::createResponse
     *
     * @param request
     *
     * @return
     */
    std::shared_ptr<message> createResponse(
        const std::shared_ptr<message> &request) override;

    /**
     * @brief @see @ref SomeipInterface::createPayload
     *
     * @return
     */
    std::shared_ptr<payload> createPayload() override;

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
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        major_version_t majorVersion = DEFAULT_MAJOR,
        event_t event = ANY_EVENT) override;

   /**
    * @brief @see @ref SomeipInterface::unSubscribe
    *
    * @param service
    * @param instance
    * @param eventgroup
    */
    void unsubscribe(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup) override;

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
        service_t service,
        instance_t instance,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) const override;

    /**
     * @brief @see @ref SomeipInterface::send
     *
     * @param message
     */
    void send(std::shared_ptr<message> message) override;

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
        service_t service,
        instance_t instance,
        event_t event,
        std::shared_ptr<payload> payload,
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
        service_t service,
        instance_t instance,
        method_t method,
        message_handler_t const &handler) override;

   /**
    * @brief @see @ref SomeipInterface::unregisterMessageHandler
    *
    * @param service
    * @param instance
    * @param method
    */
    void unregisterMessageHandler(
        service_t service,
        instance_t instance,
        method_t method) override;

    /**
     * @brief @see @ref SomeipInterface::registerAvailabilityHandler
     *
     * @param service
     * @param instance
     * @param handler
     * @param majorVersion
     * @param minorVersion
     */
    void registerAvailabilityHandler(service_t service,
        instance_t instance,
        availability_handler_t const &handler,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) override;

   /**
    * @brief @see @ref SomeipInterface::unregisterAvailabilityHandler
    *
    * @param service
    * @param instance
    * @param majorVersion
    * @param minorVersion
    */
    void unregisterAvailabilityHandler(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) override;

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
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        event_t event,
        subscription_status_handler_t handler,
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
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        event_t event) override;

    /**
    * @brief @see @ref SomeipInterface::registerSubscriptionHandler
    *
    * @param service
    * @param instance
    * @param eventgroup
    * @param handler
    */
    void registerSubscriptionHandler(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        subscription_handler_sec_t const &handler) override;

    /**
    * @brief @see @ref SomeipInterface::unregisterSubscriptionHandler
    *
    * @param service
    * @param instance
    * @param eventgroup
    */
    void unregisterSubscriptionHandler(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup) override;

    /**
    * @brief @see @ref SomeipInterface::registerStateHandler
    *
    * @param handler
    */
    void registerStateHandler(state_handler_t const &handler) override;

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
    bool isMethod(const method_t &methodId) const override;

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
