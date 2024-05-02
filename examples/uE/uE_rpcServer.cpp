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

#include "common.hpp"
#include <up-client-vsomeip-cpp/utils/Utils.hpp>
#include <condition_variable>

class RpcListener;

void registerListener(RpcListener &listner);
std::shared_ptr<UUri> getListenerURI() ;

class RpcListener : public UListener {

    UStatus sendRPCResponse(UMessage &responseMessage, uprotocol::v1::UUID &reqId) const {
        UAttributesBuilder builder = UAttributesBuilder::response(
        responseMessage.attributes().source(),
        responseMessage.attributes().sink(),
        responseMessage.attributes().priority(),
        responseMessage.attributes().id()
        );

        builder.setReqid(reqId);
        UAttributes newAttributes = builder.build();
        responseMessage.setAttributes(newAttributes);

        return VsomeipUTransport::instance().send(responseMessage);
    }

    UStatus onReceive(UMessage &message) const  {
        spdlog::info("{} - payload received from mE = {}",__FUNCTION__, reinterpret_cast<const char *>(message.payload().data()) );
        uprotocol::v1::UUID reqId = message.attributes().id();

        static uint8_t vehicleSpeed[4] = "100";

        //Create UPayload
        UPayload response(vehicleSpeed, sizeof(vehicleSpeed), UPayloadType::VALUE);

        auto type = UMessageType::UMESSAGE_TYPE_RESPONSE;
        auto priority = UPriority::UPRIORITY_CS0;

        spdlog::info("{} Sending response {} back to mE",__FUNCTION__, reinterpret_cast<char *>(vehicleSpeed) );

        //Create UAttribute
        UAttributesBuilder builder(message.attributes().sink(), message.attributes().id(), type, priority);
        builder.setSink(message.attributes().source());
        UAttributes attributes = builder.build();

        //Create Response UMessage to respond
        UMessage responsemessage(response, attributes);

        return sendRPCResponse(responsemessage, reqId);
    }
};

std::shared_ptr<UUri> getListenerURI() {
    uint32_t streamerUEntityID = 0x1;
    std::string streamerUEntityName = "1";
    uint32_t streamerUResourceID = 0x1;
    std::string streamerUResourceName = "1";

    auto u_entity = uprotocol::uri::BuildUEntity().setId(streamerUEntityID).setName(streamerUEntityName).build();
    auto u_resource = uprotocol::uri::BuildUResource().setName(streamerUResourceName).setID(streamerUResourceID).build();
    auto uri = uprotocol::uri::BuildUUri().setEntity(u_entity).setResource(u_resource).build();

    return std::make_shared<UUri>(uri);
}

void registerListener(RpcListener &listner) {
    SPDLOG_INFO("{}", __FUNCTION__);

    std::shared_ptr<UUri> listenerURI = getListenerURI();

    if (listenerURI != nullptr) {
        if (UCode::OK != VsomeipUTransport::instance().registerListener(*listenerURI, listner).code()) {
            SPDLOG_ERROR("VsomeipUTransport::instance().registerListener failed");
        } else {
            SPDLOG_INFO("VsomeipUTransport::instance().registerListener success");
        }
    }
}

int main() {
    spdlog::info("start test : mE sends rpc request and uE responds...");

    //Register common listener with up-vsomeip library
    RpcListener rpcListener;
    registerListener(rpcListener);

    //Wait infinite until user manually terminates the application
    while (true) {
        sleep(1);
    }

    spdlog::info("Exiting from vsomeip client...");
    VsomeipUTransport::instance().terminate();
    return 0;
}
