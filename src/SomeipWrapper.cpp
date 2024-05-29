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

#include <up-client-vsomeip-cpp/routing/SomeipWrapper.hpp>
#include <up-client-vsomeip-cpp/utils/Logger.hpp>
using namespace vsomeip_v3;

/**
 * @brief @see @ref SomeipInterface::getName
 *
 * @return
 */
std::string const &SomeipWrapper::getName() const {
    return app_->get_name();
}

/**
 * @brief @see @ref SomeipInterface::getClient
 *
 * @return
 */
client_t SomeipWrapper::getClient() const {
    return app_->get_client();
}

/**
 * @brief @see @ref SomeipInterface::init
 *
 * @return
 */
bool SomeipWrapper::init() {
    SPDLOG_INFO("{}", __FUNCTION__);
    app_ = vsomeip::runtime::get()->create_application("uStreamer-client");
    if (nullptr == app_) {
        SPDLOG_ERROR("{} vsomeip create_application failed !!!", __FUNCTION__);
        return false;
    } else {
        SPDLOG_INFO("{} vsomeip create_application succeeded !!!", __FUNCTION__);
    }
    bool const bok = app_->init();
    if (bok) {
        SPDLOG_INFO("{} vsomeip init succeeded !!!", __FUNCTION__);
    } else {
        SPDLOG_ERROR("{} vsomeip init failed !!!", __FUNCTION__);
    }
    return bok;
}

/**
 * @brief @see @ref SomeipInterface::start
 */
void SomeipWrapper::start() {
    SPDLOG_INFO("{}", __FUNCTION__);
    someipEventLoopThread_ = std::move(std::thread(std::bind(SomeipWrapper::eventLoopFn, this)));
}

/**
 * @brief @see @ref SomeipInterface::stop
 */
void SomeipWrapper::stop() {
    SPDLOG_INFO("{}", __FUNCTION__);
    app_->stop();
    SPDLOG_INFO("{} Before someip event loop join", __FUNCTION__);
    someipEventLoopThread_.join();
    SPDLOG_INFO("{} After someip event loop join", __FUNCTION__);
    app_ = nullptr;
}

/**
 * @brief @see @ref SomeipInterface::eventLoopFn
 *
 * @param sipw
 */
void SomeipWrapper::eventLoopFn(SomeipWrapper* sipw) {
    SPDLOG_INFO("{} Starting a new thread for vsomeip event loop", __FUNCTION__);
    sipw->app_->start();
}

/**
  *
  * @brief @see @ref SomeipInterface::offerEvent
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
void SomeipWrapper::offerEvent(
    service_t service,
    instance_t instance,
    event_t eventID,
    std::set<eventgroup_t> const &eventgroups,
    event_type_e type,
    std::chrono::milliseconds cycle,
    bool change_resets_cycle,
    bool update_on_change,
    epsilon_change_func_t const &epsilon_change_func,
    reliability_type_e reliability) {

    app_->offer_event(
        service,
        instance,
        eventID,
        eventgroups,
        type, cycle,
        change_resets_cycle,
        update_on_change,
        epsilon_change_func,
        reliability);
}

/**
 * @brief @see @ref SomeipInterface::offerService
 *
 * @param service
 * @param instance
 * @param majorVersion
 * @param minorVersion
 */
void SomeipWrapper::offerService(
    service_t service,
    instance_t instance,
    major_version_t majorVersion,
    minor_version_t minorVersion) {

    app_->offer_service(service, instance, majorVersion, minorVersion);
}

/**
 * @brief @see @ref SomeipInterface::stopOfferService
 *
 * @param service
 * @param instance
 * @param majorVersion
 * @param minorVersion
 */
void SomeipWrapper::stopOfferService(
    service_t service,
    instance_t instance,
    major_version_t majorVersion,
    minor_version_t minorVersion) {

    app_->stop_offer_service(service, instance, majorVersion, minorVersion);
}

/**
 * @brief @see @ref SomeipInterface::requestService
 *
 * @param service
 * @param instance
 * @param majorVersion
 * @param minorVersion
 */
void SomeipWrapper::requestService(
    service_t service,
    instance_t instance,
    major_version_t majorVersion,
    minor_version_t minorVersion) {

    app_->request_service(service, instance, majorVersion, minorVersion);
}

/**
 * @brief @see @ref SomeipInterface::releaseService
 *
 * @param service
 * @param instance
 */
void SomeipWrapper::releaseService(
    service_t service,
    instance_t instance) {

    app_->release_service(service, instance);
}

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
void SomeipWrapper::requestEvent(
    service_t service,
    instance_t instance,
    event_t event,
    std::set<eventgroup_t> const &eventgroups,
    event_type_e type,
    reliability_type_e reliability) {

    app_->request_event(service, instance, event, eventgroups, type, reliability);
}

/**
 * @brief @see @ref SomeipInterface::releaseEvent
 *
 * @param service
 * @param instance
 * @param event
 */
void SomeipWrapper::releaseEvent(
    service_t service,
    instance_t instance,
    event_t event) {

    app_->release_event(service, instance, event);
}

/**
 * @brief @see @ref SomeipInterface::createRequest
 *
 * @return
 */
std::shared_ptr<message> SomeipWrapper::createRequest() {
    return vsomeip::runtime::get()->create_request();
}

/**
 * @brief @see @ref SomeipInterface::createResponse
 *
 * @param request
 *
 * @return
 */
std::shared_ptr<message> SomeipWrapper::createResponse(
        const std::shared_ptr<message> &request) {
    if (!request) {
        SPDLOG_ERROR("request is null");
        return nullptr;
    } 
    else {
        return vsomeip::runtime::get()->create_response(request);
    }
}

/**
 * @brief @see @ref SomeipInterface::createPayload
 *
 * @return
 */
std::shared_ptr<payload> SomeipWrapper::createPayload() {
    return vsomeip::runtime::get()->create_payload();
}

/**
 * @brief @see @ref SomeipInterface::subscribe
 *
 * @param service
 * @param instance
 * @param eventgroup
 * @param majorVersion
 * @param event
 */
void SomeipWrapper::subscribe(
    service_t service,
    instance_t instance,
    eventgroup_t eventgroup,
    major_version_t majorVersion,
    event_t event) {

    app_->subscribe(service, instance, eventgroup, majorVersion, event);
}

/**
 * @brief @see @ref SomeipInterface::unSubscribe
 *
 * @param service
 * @param instance
 * @param eventgroup
 */
void SomeipWrapper::unsubscribe(
    service_t service,
    instance_t instance,
    eventgroup_t eventgroup) {

    app_->unsubscribe(service, instance, eventgroup);
}

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
bool SomeipWrapper::isAvailable(
    service_t service,
    instance_t instance,
    major_version_t majorVersion,
    minor_version_t minorVersion) const {

    return app_->is_available(service, instance, majorVersion, minorVersion);
}

/**
 * @brief @see @ref SomeipInterface::send
 *
 * @param message
 */
void SomeipWrapper::send(std::shared_ptr<message> message) {
    if (!message) {
        SPDLOG_ERROR("message is empty");
    } else {
        app_->send(message);
    }
}

/**
 * @brief @see @ref SomeipInterface::notify
 *
 * @param service
 * @param instance
 * @param event
 * @param payload
 * @param force
 */
void SomeipWrapper::notify(
    service_t service,
    instance_t instance,
    event_t event,
    std::shared_ptr<payload> payload,
    bool force) const {

    app_->notify(service, instance, event, payload, force);
}

/**
 * @brief @see @ref SomeipInterface::registerMessageHandler
 *
 * @param service
 * @param instance
 * @param method
 * @param handler
 */
void SomeipWrapper::registerMessageHandler(
    service_t service,
    instance_t instance,
    method_t method,
    message_handler_t const &handler) {

    app_->register_message_handler(service, instance, method, handler);
}

/**
 * @brief @see @ref SomeipInterface::unregisterMessageHandler
 *
 * @param service
 * @param instance
 * @param method
 */
void SomeipWrapper::unregisterMessageHandler(
    service_t service,
    instance_t instance,
    method_t method) {

    app_->unregister_message_handler(service, instance, method);
}

/**
 * @brief @see @ref SomeipInterface::registerAvailabilityHandler
 *
 * @param service
 * @param instance
 * @param handler
 * @param majorVersion
 * @param minorVersion
 */
void SomeipWrapper::registerAvailabilityHandler(
    service_t service,
    instance_t instance,
    availability_handler_t const &handler,
    major_version_t majorVersion,
    minor_version_t minorVersion) {

    app_->register_availability_handler(service, instance, handler, majorVersion, minorVersion);
}

/**
 * @brief @see @ref SomeipInterface::unregisterAvailabilityHandler
 *
 * @param service
 * @param instance
 * @param majorVersion
 * @param minorVersion
 */
void SomeipWrapper::unregisterAvailabilityHandler(
    service_t service,
    instance_t instance,
    major_version_t majorVersion,
    minor_version_t minorVersion) {

    app_->unregister_availability_handler(service, instance, majorVersion, minorVersion);
}

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
void SomeipWrapper::registerSubscriptionStatusHandler(
    service_t service,
    instance_t instance,
    eventgroup_t eventgroup,
    event_t event,
    subscription_status_handler_t handler,
    bool is_selective) {

    app_->register_subscription_status_handler(service, instance, eventgroup, event, handler, is_selective);
}

/**
 * @brief @see @ref SomeipInterface::unregisterSubscriptionStatusHandler
 *
 * @param service
 * @param instance
 * @param eventgroup
 * @param event
 */
void SomeipWrapper::unregisterSubscriptionStatusHandler(
    service_t service,
    instance_t instance,
    eventgroup_t eventgroup,
    event_t event) {

    app_->unregister_subscription_status_handler(service, instance, eventgroup, event);
}

/**
 * @brief @see @ref SomeipInterface::registerSubscriptionHandler
 *
 * @param service
 * @param instance
 * @param eventgroup
 * @param handler
 */
void SomeipWrapper::registerSubscriptionHandler(
    service_t service,
    instance_t instance,
    eventgroup_t eventgroup,
    subscription_handler_sec_t const &handler) {

    app_->register_subscription_handler(service, instance, eventgroup, handler);
}

/**
 * @brief @see @ref SomeipInterface::unregisterSubscriptionHandler
 *
 * @param service
 * @param instance
 * @param eventgroup
 */
void SomeipWrapper::unregisterSubscriptionHandler(
    service_t service,
    instance_t instance,
    eventgroup_t eventgroup) {

    app_->unregister_subscription_handler(service, instance, eventgroup);
}

/**
 * @brief @see @ref SomeipInterface::registerStateHandler
 *
 * @param handler
 */
void SomeipWrapper::registerStateHandler(state_handler_t const &handler) {
    app_->register_state_handler(handler);
}

/**
 * @brief @see @ref SomeipInterface::unregisterStateHandler
 */
void SomeipWrapper::unregisterStateHandler() {
    app_->unregister_state_handler();
}

/**
 * @brief @see @ref SomeipInterface::clearAllHandler
 */
void SomeipWrapper::clearAllHandler() {
    app_->clear_all_handler();
}

/**
 * @brief @see @ref SomeipInterface::isMethod
 *
 * @param methodId
 * @return true
 * @return false
 */
bool SomeipWrapper::isMethod(const method_t &methodId) const {
    //TODO fix this logic to test within method boundaries
    if ((methodId >= SOMEIP_EVENT_ID_RANGE_START) &&
        (methodId <= SOMEIP_EVENT_ID_RANGE_END)) {
        return false;
    }
    return  true;
}
