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

#ifndef SOMEIP_INTERFACE_HPP
#define SOMEIP_INTERFACE_HPP
#include "SomeipLibIncludes.hpp"

/**
 * @Note: In someip, It is common practise and recommended to split the ID space of the Method ID
 * between Methods and Events/Notifications. Methods would be in the range 0x0000-
 * 0x7FFF (first bit of Method-ID is 0) and Events/Notifications would use the range
 * 0x8000-0x8FFF (first bit of the Method-ID is 1)
 */
constexpr uint16_t SOMEIP_EVENT_ID_RANGE_START = 0x8000;
constexpr uint16_t SOMEIP_EVENT_ID_RANGE_END = 0x8FFF;

/**
 * @brief Provides the interface for Someip classes.
 *
 */
class SomeipInterface {
public:
    /**
     * @brief Constructor which takes reference to SomeipInterface.
     */
    SomeipInterface() = default;

    /**
     * @brief Destructor stops and releases the services.
     */
    virtual ~SomeipInterface() = default;

    /**
     * @brief Deleted copy constructor.
     *
     * This declaration prevents the creation of a copy constructor for the SomeipInterface class.
     * Copying instances of SomeipInterface is not allowed.
     *
     * @param SomeipInterface The instance of SomeipInterface to be copied.
     */
    SomeipInterface(SomeipInterface&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     *
     * This declaration prevents the assignment of one SomeipInterface object to another.
     * Assigning instances of SomeipInterface is not allowed.
     *
     * @param The instance of SomeipInterface to be assigned.
     * @return A reference to the current SomeipInterface object.
     */
    SomeipInterface& operator=(const SomeipInterface&) = delete;

    /**
     * @brief Deleted move constructor.
     *
     * This declaration prevents the creation of a move constructor for the SomeipInterface class.
     * Moving instances of SomeipInterface is not allowed.
     *
     * @param The rvalue reference to the instance of SomeipInteraface to be moved.
     */
     SomeipInterface(SomeipInterface&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     *
     * This declaration prevents the assignment of one SomeipInterface object to another using move semantics.
     * Moving instances of SomeipInterface is not allowed.
     *
     * @param The rvalue reference to the instance of SomeipInterface to be moved.
     * @return A reference to the current SomeipInterface object.
     */
     SomeipInterface& operator=(SomeipInterface&&) = delete;

    /**
     * @brief Returns the name of the application as given during creation
     *
     * The application name is used to identify the application. It is either
     * set explicitely when the application object is created or configured by
     * the environment variable SOMEIP_APPLICATION_NAME.
     *
     * Note: A user application can use several someip application objects in
     * parallel. The application names must be set explicitly in this case
     * because SOMEIP_APPLICATION_NAME only allows to specify a single name.
     *
     * @return Application name
     *
     */
    virtual std::string const & getName() const = 0;

    /**
     * @brief Returns the client identifier that was assigned to the
     * application object.
     *
     * Each request sent by and each response sent to the application contain
     * the client identifier as part of the request identifier within the
     * SOME/IP message header. The client identifier can either be configured
     * by the configured as part of the application node within a someip
     * configuration file or is automatically set to an unused client
     * identifier by someip. If the client identifier is automatically set,
     * its high byte will always match the diagnosis address of the device.
     *
     * @return Client ID of application
     *
     */
    virtual client_t getClient() const = 0;

    /**
     * @brief Initializes the application.
     *
     * @note The init method must be called first after creating a vsomeip
     *  application and executes the following steps to initialize it:
     * - Loading the configuration from a dynamic module
     *   - Loading the configuration from a .json file or
     *   - Loading the configuration from compiled data (not yet available)
     * - Determining routing configuration and initialization of the routing
     *   itself
     * - Installing signal handlers
     *
     * @return True vsomeip application was initialized suceessfully
     * @return False vsomeip application was initialization failed
     *
     */
    virtual bool init() = 0;

    /**
     * @brief Starts message processing.
     * @note This method must be called after init to start message processing. It
     * will block until the message processing is terminated using the @ref
     * stop method or by receiving signals. It processes messages received
     * via the sockets and uses registered callbacks to pass them to the user
     * application.
     *
     */
    virtual void start() = 0;

    /**
     * @brief Stops message processing.
     *
     * @note This method stops message processing. Thus, @ref start will return
     * after a call to stop.
     *
     */
    virtual void stop() = 0;

    /* Someip Server related interfaces (When acting like a server) */
    /**
     * @brief Offers a SOME/IP service instance.
     *
     * @note The user application must call this method for each service it offers
     * to register it at the someip routing component, which makes the
     * service visible to interested clients. Dependent on the configuration
     * the service is available internally only or internally and externally.
     * To offer a service to the external network, the configuration must
     * contain a port for the offered service instance. If no such port
     * configuration is provided, the service is not visible outside the
     * device.
     *
     * @param service Service identifier of the offered service interface.
     * @param instance Instance identifier of the offered service instance.
     * @param majorVersion Major service version (Default: 0).
     * @param minorVersion Minor service version (Default: 0).
     *
     */
    virtual void offerService(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = DEFAULT_MAJOR,
        minor_version_t minorVersion = DEFAULT_MINOR) = 0;

    /**
     * @brief Stops offering a SOME/IP service instance.
     *
     * @note The user application must call this method to withdraw a service offer.
     *
     * @param service Service identifier of the offered service interface.
     * @param instance Instance identifer of the offered service instance.
     * @param majorVersion Major service version (Default: 0).
     * @param minorVersion Minor service version (Default: 0).
     *
     */
    virtual void stopOfferService(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = DEFAULT_MAJOR,
        minor_version_t minorVersion = DEFAULT_MINOR) = 0;

    /* Someip Client related interfaces (When acting like a client) */

    /**
     * @brief Registers the application as client of a service instance.
     *
     * @note A user application must call this method for each service instance it
     * wants to use. The request is stored within the routing component and the
     * application is registered as client for the service as soon as the
     * service instance becomes available.
     *
     * @param service Service identifier of the requested service interface.
     * @param instance Instance identifier of the requested service instance.
     * @param majorVersion Major service version (Default: 0xFF).
     * @param minorVersion Minor service version (Default: 0xFFFFFF).
     *
     */
    virtual void requestService(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) = 0;

    /**
     * @brief Unregister the application as client of a service instance.
     *
     * @note A user application should call this method if it does not request to
     * use the service instance any longer. The method unregisters the request
     * a the routing component, which removes the service instance from the
     * list of requested service instances if the call releases the last
     * existing request for the service instance. This is important for
     * external service instances, as the SOME/IP Service Discovery can avoid
     * to send unnecessary Find messages.
     *
     * @param service Service identifier of the offered service interface.
     * @param instance Instance identifier of the offered service instance.
     *
     */
    virtual void releaseService(service_t service, instance_t instance) = 0;

    /**
     *
     * @brief Offers a SOME/IP event or field.
     *
     * @note A user application must call this method for each event/field it wants
     * to offer. The event is registered at the vsomeip routing component that
     * enables other applications to subscribe to the event/field as well as
     * to get and set the field value.
     *
     * @param _service Service identifier of the interface containing the
     * event.
     * @param _instance Instance identifier of the interface containing the
     * event.
     * @param _notifier Identifier of the notifier for the offered event.
     * @param _eventgroups List of eventgroup identifiers of the eventgroups
     * that contain the event.
     * @param _type Pure event, selective event or field.
     * @param _cycle Cycle time for periodic events
     * @param _change_resets_cycle Set to true if a change of the event
     * payload shall reset the cycle time for periodically sent events
     * @param _update_on_change Set to false if a change of the event should not
     * trigger a notification
     * @param _epsilon_change_func Function to determine whether two event
     * payloads are considered different
     * @param _reliability Either RT_UNRELIABLE or RT_RELIABLE. All events
     * of the same eventgroup are required to be offered with the same
     * reliability. If set to RT_BOTH remote subscriptions are required to
     * contain a reliable and unreliable endpoint option to be accepted.
     * If set to RT_UNKNOWN the event reliability is determined by the service
     * instance's reliability configuration. This parameter has no effect for
     * events of local services.
     */
    virtual void offerEvent(service_t service, instance_t instance,
                event_t eventID,
                const std::set<eventgroup_t> &_eventgroups,
                event_type_e _type = event_type_e::ET_EVENT,
                std::chrono::milliseconds _cycle = std::chrono::milliseconds::zero(),
                bool _change_resets_cycle = false,
                bool _update_on_change = true,
                const epsilon_change_func_t &_epsilon_change_func = nullptr,
                reliability_type_e _reliability = reliability_type_e::RT_UNKNOWN) = 0;

    /**
     *
     * @brief Registers the application as user of an event or field.
     *
     * @note A user application must call this method before being able to receive
     * event or field data. The method registers the event or field at the
     * routing component.
     *
     * @param service Service identifier of the interface that contains the
     * event.
     * @param instance Instance identifier of the interface that contains the
     * event.
     * @param event Event identifier of the event.
     * @param eventgroups List of Eventgroup identifiers of the eventgroups
     * that contain the event.
     * @param type Pure event, selective event or field.
     * @param reliability Either RT_UNRELIABLE or RT_RELIABLE. All events
     * of the same eventgroup are required to be requested with the same
     * reliability. If set to RT_BOTH remote subscriptions are only done if
     * the remote service is offered with an unreliable and reliable endpoint
     * option. If set to RT_UNKNOWN the event reliability is determined by the
     * service instance's reliability configuration. This parameter has no
     * effect for events of local services
     *
     * Note: If a someip application registers to an event or field and other
     * someip application shall be enabled to use events/fields from the same
     * eventgroup, the application must register all events and fields that
     * belong to the eventgroup. Otherwise, neither event type nor reliability
     * type are known which might result in missing events.
     */
    virtual void requestEvent(
        service_t service,
        instance_t instance,
        event_t event,
        std::set<eventgroup_t> const &eventgroups,
        event_type_e type = event_type_e::ET_EVENT,
        reliability_type_e reliability = reliability_type_e::RT_UNKNOWN) = 0;

    /**
     * @brief Unregister the application as user of an event or field.
     *
     * @note Unregister the application as user of an event or field and completely
     *  removes the event/field if the application is the last existing user.
     *
     * @param service Service identifier of the interface that contains the
     * event or field.
     * @param instance Instance identifier of the instance that contains the
     * event or field.
     * @param event Event identifier of the event or field.
     *
     */
    virtual void releaseEvent(
        service_t service,
        instance_t instance,
        event_t event) = 0;

    /**
     * @brief Create a Request object
     *
     * @return std::shared_ptr<message>
     *
     */
    virtual std::shared_ptr<message> createRequest() = 0;

    /**
     * @brief Create a Response object
     *
     * @param request
     * @return std::shared_ptr<message>
     *
     */
    virtual std::shared_ptr<message> createResponse(
        const std::shared_ptr<message> &request) = 0;

    /**
     * @brief Create a Payload object
     *
     * @return std::shared_ptr<payload>
     *
     */
    virtual std::shared_ptr<payload> createPayload() = 0;

    /**
     * @brief Subscribes to an eventgroup.
     *
     * @note A user application must call this function to subscribe to an eventgroup.
     * Before calling subscribe it must register all events it interested in by
     * calls to @ref requestEvent. The method additionally allows to specify
     * a specific event. If a specific event is specified, all other events of
     * the eventgroup are not received by the application.
     *
     * Note: For external services, providing a specific event does not change
     * anything regarding the message routing. The specific event is only used
     * to filter incoming events and to determine which initial events must be
     * sent.
     *
     * @param service Service identifier of the service that contains the
     * eventgroup.
     * @param instance Instance identifier of the service that contains the
     * eventgroup.
     * @param eventgroup Eventgroup identifier of the eventgroup.
     * @param majorVersion Major version number of the service.
     * @param event All (Default) or a specific event.
     *
     */
    virtual void subscribe(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        major_version_t majorVersion = DEFAULT_MAJOR,
        event_t event = ANY_EVENT) = 0;

    /**
     * @brief Unsubscribes from an eventgroup.
     *
     * @param service Service identifier of the service that contains the
     * eventgroup.
     * @param instance Instance identifier of the service that contains the
     * eventgroup.
     * @param eventgroup Eventgroup identifier of the eventgroup.
     *
     */
    virtual void unsubscribe(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup) = 0;

    /**
     * @brief Retrieve for the availability of a service instance.
     *
     * @note If the version is also given, the result will only be true if the
     * service instance is available in that specific version.
     *
     * @param service Service identifier of the service instance.
     * @param instance Instance identifier of the service instance.
     * @param majorVersion Major interface version. Use ANY_MAJOR to ignore the
     * majorVersion version.
     * @param minorVersion Minor interface version. Use ANY_MINOR to ignore the
     * minorVersion version.
     * @return True
     * @return False
     *
     */
    virtual bool isAvailable(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) const = 0;

    /* Common interfaces (irrespective of being server or client) */
    /**
     * @brief Sends a message.
     *
     * @note Serializes the specified message object, determines the target and sends
     * the message to the target. For requests, the request identifier is
     * automatically built from the client identifier and the session
     * identifier.
     *
     * @param message Message object.
     *
     */
    virtual void send(std::shared_ptr<message> message) = 0;

    /**
     * @brief Fire an event or field notification.
     *
     * @note The specified event is updated with the specified payload data.
     * Dependent on the type of the event, the payload is distributed to all
     * notified clients (always for events, only if the payload has changed
     * for fields).
     *
     * Note: Prior to using this method, @ref offer_event has to be called by
     * the service provider.
     *
     * @param service Service identifier of the service that contains the
     * event.
     * @param instance Instance identifier of the service instance that
     * holds the event.
     * @param event Event identifier of the event.
     * @param payload Serialized payload of the event.
     * @param force states if the notif be forced
     *
     */
    virtual void notify(
        service_t service,
        instance_t instance,
        event_t event,
        std::shared_ptr<payload> payload,
        bool force = false) const = 0;

    /**
     * @brief Registers a handler for the specified method or event.
     *
     * @note A user application must call this method to register callbacks for
     * for messages that match the specified service, instance, method/event
     * pattern. It is possible to specify wildcard values for all three
     * identifiers arguments.
     *
     * Notes:
     * - Only a single handler can be registered per service, instance,
     *   method/event combination.
     * - A subsequent call will overwrite an existing registration.
     * - Handler registrations containing wildcards can be active in parallel
     *   to handler registrations for specific service, instance, method/event
     *   combinations.
     *
     * @param service Service identifier of the service that contains the
     * method or event. Can be set to ANY_SERVICE to register a handler for
     * a message independent from a specific service.
     * @param instance Instance identifier of the service instance that
     * contains the method or event. Can be set to ANY_INSTANCE to register
     * a handler for a message independent from a specific service.
     * @param method Method/Event identifier of the method/event that is
     * to be handled. Can be set to ANY_METHOD to register a handler for
     * all methods and events.
     * @param handler Callback that will be called if a message arrives
     * that matches the specified service, instance and method/event
     * parameters.
     *
     */
    virtual void registerMessageHandler(
        service_t service,
        instance_t instance,
        method_t method,
        message_handler_t const &handler) = 0;

    /**
     * @brief Unregisters the message handler for the specified service
     * method/event notification.
     *
     * @param service Service identifier of the service that contains the
     * method or event. Can be set to ANY_SERVICE to unregister a handler for
     * a message independent from a specific service.
     * @param instance Instance identifier of the service instance that
     * contains the method or event. Can be set to ANY_INSTANCE to unregister
     * a handler for a message independent from a specific service.
     * @param method Method/Event identifier of the method/event that is
     * to be handled. Can be set to ANY_METHOD to unregister a handler for
     * all methods and events.
     *
     */
    virtual void unregisterMessageHandler(
        service_t service,
        instance_t instance,
        method_t method) = 0;

    /**
     * @brief Register a callback that is called when service instances
     * availability changes.
     *
     * @note This method allows for the registration of callbacks that are called
     * whenever a service appears or disappears. It is possible to specify
     * wildcards for service, instance and/or version. Additionally, the
     * version specification is optional and defaults to DEFAULT_MAJOR
     * /DEFAULT_MINOR.
     *
     * @param service Service identifier of the service instance whose
     * availability shall be reported. Can be set to ANY_SERVICE.
     * @param instance Instance identifier of the service instance whose
     * availability shall be reported. Can be set to ANY_INSTANCE.
     * @param handler Callback to be called if availability changes.
     * @param majorVersion Major service version. The parameter defaults to
     * DEFAULT_MAJOR and can be set to ANY_MAJOR.
     * @param minorVersion Minor service version. The parameter defaults to
     * DEFAULT_MINOR and can be set to ANY_MINOR.
     *
     */
    virtual void registerAvailabilityHandler(
        service_t service,
        instance_t instance,
        availability_handler_t const &handler,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) = 0;

    /**
     * @brief Unregister an availability callback.
     *
     * @param service Service identifier of the service instance whose
     * availability shall be reported. Can be set to ANY_SERVICE.
     * @param instance Instance identifier of the service instance whose
     * availability shall be reported. Can be set to ANY_INSTANCE.
     * @param majorVersion Major service version. The parameter defaults to
     * DEFAULT_MAJOR and can be set to ANY_MAJOR.
     * @param minorVersion Minor service version. The parameter defaults to
     * DEFAULT_MINOR and can be set to ANY_MINOR.
     *
     */
    virtual void unregisterAvailabilityHandler(
        service_t service,
        instance_t instance,
        major_version_t majorVersion = ANY_MAJOR,
        minor_version_t minorVersion = ANY_MINOR) = 0;

    /**
     * @brief Registers a subscription status listener.
     *
     * @note When registered such a handler it will be called for
     * every application::subscribe call.
     *
     * The handler is called whenever a subscription request for an eventgroup
     * and/or event was either accepted or rejected. The respective callback is
     * called with either OK (0x00) or REJECTED (0x07).
     *
     * @param service Service identifier of the service that is subscribed to.
     * @param instance Instance identifier of the service that is subscribed to.
     * @param eventgroup Eventgroup identifier of the eventgroup is subscribed to.
     * @param event Event indentifier of the event is subscribed to.
     * @param handler A subscription status handler which will be called by someip
     * as a follow of application::subscribe.
     * @param is_selective Flag to enable calling the provided handler if the
     * subscription is answered with a SUBSCRIBE_NACK.
     *
     */
    virtual void registerSubscriptionStatusHandler(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        event_t event,
        subscription_status_handler_t handler,
        bool is_selective = false) = 0;

    /**
     * @brief Unregisters a subscription status listener.
     *
     * @param service Service identifier of the service for which the handler
     * should be removed.
     * @param instance Instance identifier of the service for which the handler
     * should be removed
     * @param eventgroup Eventgroup identifier of the eventgroup for which the
     * should be removed
     * @param event Event identifier of the event for which the handler should
     * be removed.
     *
     */
    virtual void unregisterSubscriptionStatusHandler(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        event_t event) = 0;

    /**
     * @brief Registers a subscription handler.
     *
     * @note A subscription handler is called whenever the subscription state of an
     * eventgroup changes. The callback is called with the client identifier
     * and a boolean that indicates whether the client subscribed or
     * unsubscribed.
     * (vsomeip_sec_client_t-aware)
     *
     * @param service Service identifier of service instance whose
     * subscription state is to be monitored.
     * @param instance Instance identifier of service instance whose
     * subscription state is to be monitored.
     * @param eventgroup Eventgroup identifier of eventgroup whose
     * subscription state is to be monitored.
     * @param handler Callback that shall be called. The value returned by this
     * handler decides if the subscription is accepted or not.
     *
     */
    virtual void registerSubscriptionHandler(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup,
        subscription_handler_sec_t const &handler) = 0;

    /**
     * @brief Unregister a subscription handler.
     *
     * @param service Service identifier of service instance whose
     * subscription state is to be monitored.
     * @param instance Instance identifier of service instance whose
     * subscription state is to be monitored.
     * @param eventgroup Eventgroup identifier of eventgroup whose
     * subscription state is to be monitored.
     *
     */
    virtual void unregisterSubscriptionHandler(
        service_t service,
        instance_t instance,
        eventgroup_t eventgroup) = 0;

    /**
     * @brief Register a state handler with the vsomeip runtime.
     *
     * @note The state handler tells if this client is successfully [de]registered
     * at the central vsomeip routing component. This is called during the
     * @ref start and @ref stop methods of this class to inform the user
     * application about the registration state.
     *
     * @param handler Handler function to be called on state change.
     *
     */
    virtual void registerStateHandler(state_handler_t const &handler) = 0;

    /**
     * @brief Unregister the state handler.
     *
     */
    virtual void unregisterStateHandler() = 0;

    /**
     * @brief Unregister all registered handlers.
     *
     */
    virtual void clearAllHandler() = 0;

    /**
     * @brief Returns if the method id is an event/notification type.
     *
     * @param methodId
     * @return true
     * @return false
     */
    virtual bool isMethod(const method_t &methodId) const = 0;
};

#endif //SOMEIP_INTERFACE_HPP