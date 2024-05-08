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

#ifndef UTILS_HPP
#define UTILS_HPP
#include <condition_variable>

namespace up::vsomeip_client {
/**
 * @brief A compound variable is a variable that can be read and written
 * and can be used to notify other threads when the value is changed.
 * The variable is thread safe and can be used to synchronize threads.
 *
 * @param T
 * @param readable If the variable is readable default is false
 */
template <typename T>
class CompoundVariable {
public:
    /**
     * @brief Construct a new Variable With Status object
     *
     * @param value The value of the variable
     * @param readable If the variable is readable default is false
     */
    CompoundVariable(
        const T& value,
        const bool readable=false) :
        varValue_(value),
        isReadable_(readable) {}

    /**
     * @brief Set the Value object
     *
     * @param value
     */
    void setValue(const T& value) {
        varValue_ = value;
        setReadableStatus(true);
    }

    /**
     * @brief Set the Value and notify
     * anyone waiting for the value.
     *
     * @param value
     */
    void setValueAndNotify(const T& value) {
        std::lock_guard const lock(varMutex_);
        varValue_ = value;
        setReadableStatus(true);
        varCond_.notify_one();
    }
    /**
     * @brief Get the variable value
     *
     * @return T
     */
    T value() const { return varValue_; }
    /**
     * @brief Set the Readable Status of variable
     *
     * @param readable
     */
    void setReadableStatus(const bool readable) { isReadable_ = readable; }
    /**
     * @brief Get the Readable status of variable without any wait.
     *
     * @param readable
     */
    bool isReadable() const { return isReadable_; }

    /**
     * @brief Wait for the variable to be readable
     * with a default timeout of 60 seconds.
     *
     * @param timeout
     */
    bool isReadableWithWait(const std::chrono::milliseconds timeout=std::chrono::milliseconds(60000UL)) {
        std::unique_lock<std::mutex> lock(varMutex_);
        return (varCond_.wait_for(lock, timeout, [this] { return isReadable_; }));
    }
private:
    T varValue_;
    bool isReadable_;
    std::mutex varMutex_;
    std::condition_variable varCond_;
};


} // namespace up::vsomeip_client

#endif // UTILS_HPP