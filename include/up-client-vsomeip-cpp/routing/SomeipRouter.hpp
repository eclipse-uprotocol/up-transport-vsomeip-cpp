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

#ifndef SOMEIP_ROUTER_HPP
#define SOMEIP_ROUTER_HPP
#include <up-client-vsomeip-cpp/routing/SomeipHandler.hpp>
#include <up-cpp/transport/UTransport.h>
#include "SomeipRouterInterface.hpp"
#include <unordered_map>

/**
 * @brief Implements the SomeipRouter functionality.
 *
 * @note It provides the necessary methods to handle SomeIP routing operations.
 */
class SomeipRouter : public SomeipRouterInterface{
public:
        /**
     * @brief Constructs a SomeipRouter object with a UStreamer reference.
     * @param listener Common listner between uStreamer and vSomeip Client
     */
    SomeipRouter(uprotocol::utransport::UListener const &listener);

    /**
     * @brief Deleted copy constructor.
     *
     * @note This declaration prevents the creation of a copy constructor for the SomeipRouter class.
     * Copying instances of SomeipRouter is not allowed.
     *
     * @param other The instance of SomeipRouter to be copied.
     */
    SomeipRouter(SomeipRouter&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     *
     * @note This declaration prevents the assignment of one SomeipRouter object to another.
     * Assigning instances of SomeipRouter is not allowed.
     *
     * @param other The instance of SomeipRouter to be assigned.
     * @return A reference to the current SomeipRouter object.
     */
    SomeipRouter& operator=(const SomeipRouter&) = delete;

    /**
     * @brief Deleted move constructor.
     *
     * @note This declaration prevents the creation of a move constructor for the SomeipRouter class.
     * Moving instances of SomeipRouter is not allowed.
     *
     * @param other The rvalue reference to the instance of SomeipRouter to be moved.
     */
    SomeipRouter(SomeipRouter&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     *
     * @note This declaration prevents the assignment of one SomeipRouter object to another using move semantics.
     * Moving instances of SomeipRouter is not allowed.
     *
     * @param other The rvalue reference to the instance of SomeipRouter to be moved.
     * @return A reference to the current SomeipRouter object.
     */
    SomeipRouter& operator=(SomeipRouter&&) = delete;

    /**
     * @brief Destroys the SomeipRouter object.
     *
     * @note This destructor is responsible for cleaning up any resources allocated by the SomeipRouter object.
     * It is automatically called when the object goes out of scope or is explicitly deleted.
     *
     */
    ~SomeipRouter();

    /**
     * @brief Initializes the SomeipRouter.
     * @see PluginInterface::init()
     * @return True if the initialization is successful, false otherwise.
     */
    bool init();

    /**
     * @brief @see @ref SomeipRouterInterface::routeInboundMsg()
     * @param[in] uMessage A shared pointer to the UMessage object representing the inbound message.
     * @return True if the message was successfully routed, false otherwise
     */
    bool routeInboundMsg(uprotocol::utransport::UMessage &uMessage) override;

    /**
     * @brief @see @ref SomeipRouterInterface::getMessageTranslator()
     *
     * @return A reference to the message translator object.
     */
    MessageTranslator& getMessageTranslator() override;

    /**
     * @brief This function is used to route outbound messages
     *
     * @param[in] umsg
     *
     * @return true if outbound message is routed successfully and false otherwise
    */
    bool routeOutboundMsg(const uprotocol::utransport::UMessage &umsg);

    /**
     * @brief This function is used to route inbound subscription/unsubscription
     * request to MessageRouter for further handling.
     *
     * @param[in] strTopic Topic to subscribe
     * @param[in] isSubscribe true if subscription request and false if unsubscription request
     *
     * @return true if subscription request is successful, false otherwise
    */
    bool routeInboundSubscription(std::string const &strTopic, bool const &isSubscribe) const override;

    /**
     * @brief Handles a state change event.
     *
     * @note This method is called when a state change event occurs.
     *
     * @param[in] currentState The new state of the SomeipRouter.
     */
    void onState(state_type_e const currentState);

    /**
     * @brief @see @ref SomeipRouterInterface::isStateRegistered()
     *
     * @return True if the state is registered, false otherwise.
     */
    bool isStateRegistered() override;

private:

    /**
     * @brief Stop the service.
     *
     * @note This function stops the execution of the service.
     * It performs any necessary cleanup or shutdown procedures.
     * After calling this function, the service is no longer running.
     */
    void stopService();

    /**
     * @brief enableAllLocalServices fetch all local services and calls offerServicesAndEvents
    */
    void enableAllLocalServices();

    /**
     * @brief offerServicesAndEvents offers services and events for the given uri
    */
    void offerServicesAndEvents(std::shared_ptr<uprotocol::v1::UUri>);

    /**
     * @brief getUriList fetches the uri list for the given service type from json
     * @param serviceType - can be either "services" or "remoteServices"
     * @return list of UUri as a shared pointer to a vector
    */
    std::shared_ptr<std::vector<std::shared_ptr<uprotocol::v1::UUri>>> getUriList(const std::string &serviceType);

    /**
     * @brief Retrieves the SomeipHandler associated with the given ID.
     *
     * @param[in] UEId The unique identifier of the handler.
     * @return A shared pointer to the SomeipHandler object, or nullptr if no handler is found.
     */
    std::shared_ptr<SomeipHandler> getHandler(uint16_t UEId);

    /**
     * @brief Creates a new SomeipHandler object of the specified type and associates it with the given ID.
     *
     * @note This method creates a new SomeipHandler object based on
     * the specified handler type and associates it with the provided ID
     *
     * @param[in] type The type of the handler to create.
     * @param[in] UEId The unique identifier of the handler.
     * @return A shared pointer to the newly created SomeipHandler object.
     */
    std::shared_ptr<SomeipHandler> newHandler(
        HandlerType type,
        const uprotocol::v1::UEntity &uEntityInfo,
        const uprotocol::v1::UAuthority &uAuthorityInfo);

    /**
     * @brief Removes the SomeipHandler associated with the given ID.
     *
     * @note This method removes the SomeipHandler object that is associated with the provided "ID".
     *
     * @param[in] UEId The unique identifier of the handler to remove.
     */
    void removeHandler(uint16_t const UEId);

    /**
     * @brief A shared pointer to a SomeipInterface object.
     *
     * @note This variable represents a shared pointer to an instance of the SomeipInterface class.
     */
    std::shared_ptr<SomeipInterface> someipInterface_;

    /**
     * @brief A unique pointer to a MessageTranslator object.
     *
     * @note This variable represents a unique pointer to an instance of the MessageTranslator class.
     */
    std::shared_ptr<MessageTranslator> msgTranslator_;

    /**
     * @brief A map of SomeIP handlers.
     *
     * @note This variable represents an unordered map that associates a key with a shared pointer
     * to a SomeipHandler object. The map is used to store and manage the handlers for SomeIP messages.
     * The key is typically used to identify the specific handler.
     */
    std::unordered_map<uint16_t,std::shared_ptr<SomeipHandler>> handlers_; // key: UEId, value: handler


    /**
     * @brief A common listner between uStreamer and vSomeip Client.
    */
    const uprotocol::utransport::UListener &listener_;

    /**
     * @brief The state of the object.
     *
     * @note This variable represents the state of the object. The specific type of the state is defined by
     * the "stateType" type. The value of this variable determines the behavior or characteristics of
     * the object.
     */
    stateType state_;

};
#endif /* SOMEIP_ROUTER_HPP */
