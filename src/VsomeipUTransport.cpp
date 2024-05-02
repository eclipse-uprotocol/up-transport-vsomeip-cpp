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

#include <up-client-vsomeip-cpp/transport/VsomeipUTransport.hpp>
#include <up-client-vsomeip-cpp/utils/Logger.hpp>

using namespace uprotocol::v1;
using namespace uprotocol::utransport;

VsomeipUTransport& VsomeipUTransport::instance(void) noexcept {
    static VsomeipUTransport instance;
    return instance;
}

/**
  * @brief @return @see @ref VsomeipUTransport::init
  * @param listener @see @ref VsomeipUTransport::init
  */
UStatus VsomeipUTransport::init(const UListener &listener) noexcept {
    UStatus status;
    SPDLOG_INFO("{}", __FUNCTION__);

    try{
        router_ = std::make_unique<SomeipRouter>(listener);
    } catch (const std::bad_alloc& e) {
        SPDLOG_ERROR("Failed to allocate memory for SomeipRouter: {}", e.what());
        status.set_code(UCode::RESOURCE_EXHAUSTED);
        return status;
    }
        if (! router_->init()) {
        SPDLOG_ERROR("Failed to initialize SomeipRouter");
        status.set_code(UCode::UNAVAILABLE);
        return status;
    }

    status.set_code(UCode::OK);
    return status;
}

UStatus VsomeipUTransport::terminate() noexcept {
    UStatus status;
    SPDLOG_INFO("{}", __FUNCTION__);
    router_.reset();
    status.set_code(UCode::OK);
    return status;
}

/**
  * @brief @see @ref VsomeipUTransport::send
  * @param message @see @ref VsomeipUTransport::send
  * @return @see @ref VsomeipUTransport::send
  */
UStatus VsomeipUTransport::send(const UMessage &message) noexcept {
    UStatus status;

    if (! router_->routeOutboundMsg(message)) {
        status.set_code(UCode::UNAVAILABLE);
        return status;
    }

    status.set_code(UCode::OK);
    return status;
}

UStatus VsomeipUTransport::registerListener(const UUri &uri,
                                            const UListener &listener) noexcept {
    SPDLOG_INFO("{}", __FUNCTION__);
    static_cast<void>(uri);
    UStatus status = init(listener);
    return status;
}

UStatus VsomeipUTransport::unregisterListener(const UUri &uri,
                           const UListener &listener) noexcept {
    static_cast<void>(uri);
    static_cast<void>(listener);
    UStatus status;
    status.set_code(UCode::UNIMPLEMENTED);
    return status;
}

UStatus VsomeipUTransport::receive(const UUri &uri,
                const UPayload &payload,
                const UAttributes &attributes) noexcept {
    static_cast<void>(uri);
    static_cast<void>(payload);
    static_cast<void>(attributes);
    UStatus status;
    status.set_code(UCode::UNIMPLEMENTED);
    return status;
}
