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

#ifndef STREAMER_LOGGER_H_
#define STREAMER_LOGGER_H_
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <csignal>

void handleLogLevelSignal(int signal, siginfo_t *sip, void *ptr);
void setupLogger();

//* Sys Logging */
#define LogTrace(...)        SPDLOG_INFO(__VA_ARGS__)
#define LogInfo(...)         SPDLOG_INFO(__VA_ARGS__)
#define LogWarn(...)         SPDLOG_WARN(__VA_ARGS__)
#define LogDebug(...)        SPDLOG_INFO(__VA_ARGS__)
#define LogErr(...)          SPDLOG_ERROR(__VA_ARGS__)
#define LogCrit(...)         SPDLOG_CRITICAL(__VA_ARGS__)

#endif /* STREAMER_LOGGER_H_ */
