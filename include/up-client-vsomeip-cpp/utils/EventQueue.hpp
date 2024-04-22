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

#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <queue>
#include <mutex>

/**
 * @brief A generic event queue class.
 *
 * @note This class template represents a generic event queue that can hold events of any type.
 * It provides functionality for adding events to the queue, removing events from the queue,
 * and checking if the queue is empty.
 *
 * @tparam T The type of events to be stored in the queue.
 */
template <class T>
class EventQueue final
{
public:
    /**
     * @brief Default constructor for the EventQueue class.
     */
    EventQueue() = default;

    /**
     * @brief This function adds an event of type T to the event queue..
     *
     * @param e The event to be added to the queue.
     */
    void push(T e) {
        std::scoped_lock lock(m_Qlock);
        (void)m_EventQ.emplace(std::move(e));
    }

    /**
     * @brief Remove and return the next event from the event queue.
     *
     * @return The next event in the queue of type T.
     */
    T pop(void) {
        T temp = T();
        std::scoped_lock lock(m_Qlock);
        if (!m_EventQ.empty()) {
            temp = std::move(m_EventQ.front());
            m_EventQ.pop();
        }
        return temp;
    }

    /**
     * @brief Flushes the event queue.
     */
    void flush() {
        std::scoped_lock lock(m_Qlock);
        while (!m_EventQ.empty()) {
            m_EventQ.pop();
        }
    }

    /**
     * @brief Checks if the event queue is empty.
     *
     * @return True if the event queue is empty, false otherwise.
     */
    bool empty() {
        std::scoped_lock lock(m_Qlock);
        return m_EventQ.empty();
    }

    /**
     * @brief Returns the current size of the event queue.
     *
     * @note The current size represents the number of events stored in queue.
     *
     * @return The size of the event queue as a size_t value.
     */
    size_t size() {
        std::scoped_lock lock(m_Qlock);
        size_t qSize = m_EventQ.size();
        return qSize;
    }

private:
    /**
     * @brief Mutex for protecting access to shared resource.
     */
    std::mutex m_Qlock;

    /**
     * @brief Queue for storing events of type T.
     */
    std::queue<T> m_EventQ;
};


#endif //_EVENT_QUEUE_H_