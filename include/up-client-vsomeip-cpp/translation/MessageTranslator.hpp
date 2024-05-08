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

#ifndef MESSAGE_TRANSLATOR_HPP
#define MESSAGE_TRANSLATOR_HPP
#include <up-cpp/transport/UTransport.h>
#include <up-client-vsomeip-cpp/routing/SomeipInterface.hpp>
#include <memory>

constexpr vsomeip_v3::instance_t INSTANCE_ID_PER_SPEC  = 0x0U;

/**
* @brief MessageTranslator class is a utility class that can translate uMessage into someipMessage and vice-versa
*/
class MessageTranslator final {
public:
    /**
     * @brief Constructor for message translator class which initializes someip interface and ustreamer.
     * @param someipInterfaceObj reference to some ip interface used for translating umsg to some ip message
     * and vice versa
     */
    MessageTranslator(SomeipInterface& someipInterfaceObj);

    /**
     * @brief Translate a UMessage to a Someip message format
     * @param uMessage, pointer to uMessage which is translated to the some ip message.
     * @return shared pointer to some ip message
     */
    std::shared_ptr<vsomeip_v3::message> translateUMessageToSomeipMsgForRequest(std::shared_ptr<uprotocol::utransport::UMessage> uMessage);

    /**
     * @brief Translates a SomeIP message to a UMessage using shared pointers.
     * @param[in] someIpMessage shared pointer to the SomeIP message to be translated to UMessage.
     * @param[in] uEntity UEntity for inbound request.
     * @param[in] uResource UResource for inbound request.
     * @return UMessage shared ptr
     */
    std::shared_ptr<uprotocol::utransport::UMessage> translateSomeipToUMsgForRequest(
        std::shared_ptr<vsomeip_v3::message> const &someIpMessage,
        uprotocol::v1::UEntity const &uEntity ,
        uprotocol::v1::UResource const &uResource) const ;

    /**
     * @brief Translates a SomeIP message to a UMessage using shared pointers.
     * @param[in] someIpMessage shared pointer to the SomeIP message to be translated to UMessage.
     * @param[in] uri UUri for inbound response.
     * @param[in] uuid uuid of the request message.
     * @return shared pointer to UMessage
     */
    std::shared_ptr<uprotocol::utransport::UMessage> translateSomeipToUMsgForResponse(
        std::shared_ptr<vsomeip_v3::message> const &someIpMessage,
		std::shared_ptr<uprotocol::utransport::UMessage> originalRequestMsg) const ;

    /**
     * @brief Translates a SomeIP message to a UMessage using shared pointers.
     * @param[in] someIpMessage shared pointer to the SomeIP message to be translated to UMessage.
     * @param[in] uEntity
     * @param[in] uAuthority
     * @param[in] uResource
     * @return shared pointer to UMessage
     */
    std::shared_ptr<uprotocol::utransport::UMessage> translateSomeipToUMsgForNotification(
        std::shared_ptr<vsomeip_v3::message> const someIpMessage,
        uprotocol::v1::UEntity const &uEntity ,
        uprotocol::v1::UAuthority const &uAuthority,
        uprotocol::v1::UResource const &uResource) const ;

    /**
     * @brief Translates a SomeIP subscription ack to UMessage subscription ack.
     * @param[in] uEntity
     * @param[in] uAuthority
     * @param[in] uResource
     * @return shared pointer to UMessage
    */
    std::shared_ptr<uprotocol::utransport::UMessage> translateSomeipToUMsgForSubscriptionAck(
        uprotocol::v1::UEntity const &uEntity,
        uprotocol::v1::UAuthority const &uAuthority,
        uprotocol::v1::UResource const &uResource) const;

private:

    /**
     * @brief Create UPayload with Someip payload
     * @param[in] someIpMessage Someip payload to be passed
     *
     * @return shared pointer to UPayload
    */
    std::shared_ptr<uprotocol::utransport::UPayload> buildUPayload(std::shared_ptr<vsomeip_v3::message> const someIpMessage) const;

    /**
     * @brief Reference to someIpInterface
     */
    SomeipInterface& someipInterfaceRef_;
};

#endif /* MESSAGE_TRANSLATOR_HPP */
