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

#include <up-client-vsomeip-cpp/utils/Logger.hpp>
#include "spdlog/sinks/rotating_file_sink.h"

void setupLogger() {
    uint32_t flush_time = 1;
    uint32_t max_size = 1024*1024*10;
    uint32_t max_files = 3;

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/main.log", max_size, max_files);
    std::vector<spdlog::sink_ptr> sinks {stdout_sink, rotating_sink};
    auto logger = std::make_shared<spdlog::logger>("streamer-log", sinks.begin(), sinks.end());

    spdlog::set_default_logger(logger);
    spdlog::default_logger()->info("===================== START =====================");
    spdlog::flush_every(std::chrono::seconds(flush_time));

    spdlog::set_pattern("%^[%Y.%m.%d %H:%M:%S.%e] %P %t [%^%L%$] [%-26s]:%# %v%$"); //[%n:%l] -for log level string description
    spdlog::set_level(spdlog::level::level_enum::trace);
}
