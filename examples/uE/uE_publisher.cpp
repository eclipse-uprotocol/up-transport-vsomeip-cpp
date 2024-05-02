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


const std::string UT_SUBSCRIBE_TOPIC = "/example.cabin_climate/1.0/climate_control#cabinClimateTemperature";

const uint16_t UEntityID             = 0x1104; //Service ID
const std::string UEntityName        = "0x1104";
const uint32_t UEntityVersionMajor   = 0x1; //Major Version
const uint32_t UEntityVersionMinor   = 0x0; //Minor Version

const uint16_t UResourceID           = 0x8100; //Method ID
const std::string UResourceName      = "0x8100";
const std::string UResourceInstance  = "";

class UEListener;

std::shared_ptr<UUri> getListenerURI();
void registerListener(UEListener &listner);
UCode sendMessage(UUri &uri, uint8_t *buffer, size_t size);
std::shared_ptr<UUri> buildURI();

class UEListener : public UListener
{
    UStatus onReceive(UMessage &message) const {
        std::ignore = message;
        UStatus status;
        status.set_code(UCode::OK);
        return status;
    }
};

std::shared_ptr<UUri> buildURI() {
    std::shared_ptr<UUri> uriPtr = std::make_shared<UUri>();

    UEntity uentity;
    UResource uresource;

    //Create uEntity
    uentity.set_id(UEntityID);
    uentity.set_name(UEntityName.c_str());
    uentity.set_version_major(UEntityVersionMajor);
    uentity.set_version_minor(UEntityVersionMinor);

    uriPtr->mutable_entity()->CopyFrom(uentity);

    //Create uResource
    uresource.set_id(UResourceID);
    uresource.set_name(UResourceName.c_str());
    uresource.set_instance(UResourceInstance.c_str());

    uriPtr->mutable_resource()->CopyFrom(uresource);
    return uriPtr;
}

static uint8_t *getLedONMsg() {
    static uint8_t ledOnPayload[6] = {0,1,1,0,0};
    return ledOnPayload;
}

static uint8_t *getLedOFFMsg() {
    static uint8_t ledOnPayload[6] = {0,0,1,0,0};
    return ledOnPayload;
}

UCode sendMessage(UUri &uri, uint8_t *buffer, size_t size) {
    //Create UAttribute
    auto uuid = Uuidv8Factory::create();
    auto type = UMessageType::UMESSAGE_TYPE_PUBLISH;
    auto priority = UPriority::UPRIORITY_CS0;
    UAttributesBuilder builder(uri, uuid, type, priority);
    UAttributes attributes = builder.build();

    //Create UPayload
    UPayload payload(buffer, size, UPayloadType::VALUE);

    //Create UMessage
    UMessage message(payload, attributes);

    //Publish message to vsomeip client library
    UStatus status = VsomeipUTransport::instance().send(message);

    if (UCode::OK != status.code()) {
        spdlog::error("send failed");
        return UCode::UNAVAILABLE;
    }

    return UCode::OK;
}

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

void registerListener(UEListener &listner) {
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
    spdlog::info("start test : mE subscribes and uE publishes...");

    //Register common listener with up-vsomeip library
    UEListener listener;
    registerListener(listener);

    //Build UUri to publish message
    std::shared_ptr<UUri> ledControllerURI = buildURI();

    //Wait infinite until user terminates and send LED ON and OFF publish message for every 10 seconds
    while (true) {
        static uint8_t *temperatureDataON = getLedONMsg();
        if (UCode::OK != sendMessage(*ledControllerURI, temperatureDataON, 5)) {
            spdlog::error("sendMessage failed");
            break;
        }
        sleep(2);
        static uint8_t *temperatureDataOFF = getLedOFFMsg();
        if (UCode::OK != sendMessage(*ledControllerURI, temperatureDataOFF, 5)) {
            spdlog::error("sendMessage failed");
            break;
        }
        sleep(2);
    }

    spdlog::info("Exiting from vsomeip client...");
    VsomeipUTransport::instance().terminate();
    return 0;
}
