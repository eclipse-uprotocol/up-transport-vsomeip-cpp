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
