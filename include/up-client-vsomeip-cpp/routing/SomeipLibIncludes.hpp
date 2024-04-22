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

#ifndef SOMEIP_LIB_INCLUDES_HPP
#define SOMEIP_LIB_INCLUDES_HPP


/* TODO: LOCATION OF THIS FILE IS IMPORTANT
 * currently placing it alongside the SomeipInterface.hpp file
 */

/**
 * @brief This file should include all the required headers from the someip library
 * currently being used. This file with the same name shall exist in all the someip
 * implementations. This file is included in the SomeipInterface.hpp file.
 * This is done so that someip libraries can be swapped in and out without having to
 * compile the entire project with the new library, given that the new library is
 * compatible with the interface defined in SomeipInterface.hpp and the libraries are
 * linked dynamically (.so).
 *
 */

/* This version of SomeipLibIncludes is for vsomeip library */
#include "vsomeip/vsomeip.hpp"
#include "vsomeip/handler.hpp"
#include <vsomeip/primitive_types.hpp>

using namespace vsomeip_v3;

/**
 * @brief typecasting stateType as state_type_e
 *
 */
using stateType = state_type_e;

/**
 * @brief typecasting secClientType as vsomeip_sec_client_t
 *
 */
using secClientType = vsomeip_sec_client_t;

/**
 * @brief This function converts the stateType from enum to string
 *
 * @param enVal enum val that is to be converted into string
 * @return enum value in string format
 *
 */
constexpr const char *stateTypeAsStr(stateType const enVal) noexcept {
    const char *ret = "InvalidInput";
    switch (enVal)
    {
    case stateType::ST_REGISTERED:
        ret = "ST_REGISTERED";
        break;
    case stateType::ST_DEREGISTERED:
        ret = "ST_DEREGISTERED";
        break;

    default:
        break;
    }
    return ret;
}

#endif //SOMEIP_LIB_INCLUDES_HPP