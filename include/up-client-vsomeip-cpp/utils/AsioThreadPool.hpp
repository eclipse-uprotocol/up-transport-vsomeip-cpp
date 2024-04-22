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

#ifndef ASIO_THREAD_POOL_HPP
#define ASIO_THREAD_POOL_HPP
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <thread>

/* This is done so that in future if we stop using boost library
 * but still use asio, we can easily replace boost::asio with std::asio
 */
using namespace boost;


/**
 * @brief This class is a singleton class which provides a thread pool
 * using ::asio::thread_pool. This class is used to post tasks to the
 * thread pool. Currently it creates a thread pool with number of threads
 * equal to number of cores in the system.
 * An example to use this class is as follows:
 * @code
 * Example-1(using lambda function) :
 * AsioThreadPool::getInstance().post([]() {
 *    // Do something
 * });
 * Example-2(using std::bind) :
 * void task() {}
 * AsioThreadPool::getInstance().post(task)
 * @endcode
 *
 */
class AsioThreadPool {
public:
    static AsioThreadPool& getInstance() {
        static AsioThreadPool instance;
        return instance;
    }

    /**
     * @brief This function is used to post a task to the thread pool
     *
     * @param func
     */
    void post(const std::function<void()>& func) {
        asio::post(threadPool_, func);
    }

    /**
     * @brief This function is used to post a task to the thread pool
     *
     * @param func This function takes a function which takes an unsigned long
     * as a parameter
     */
    void post(const std::function<void(uint32_t)>& func) {
        (void)func;
        // asio::post(threadPool_, func);
    }

    /**
     * @brief This function is used to post a task to the thread pool
     *
     * @param func This function takes a function which takes an unsigned long
     * as a parameter
     */
    void post(const std::function<void(unsigned long)>& func) {
        (void)func;
        // asio::post(threadPool_, func);
    }

    /**
     * @brief This function is used to post a task to the thread pool
     *
     * @param func This function takes a function which takes a void pointer
     * as a parameter
     */
    void post(std::function<void(std::shared_ptr<void>)>& func) {
        (void)func;
        // asio::post(threadPool_, func);
    }

    /**
     * @brief This function is used to post a task to the thread pool
     *
     * @param func This function takes a function which takes an uint32_t, an unsigned long
     * and a void pointer as parameters
     */
    void post(std::function<void(uint32_t, unsigned long, std::shared_ptr<void>)>& func) {
        (void)func;
        // asio::post(threadPool_, func);
    }

private:
    AsioThreadPool() { }
    ~AsioThreadPool() { threadPool_.stop(); }
    AsioThreadPool(const AsioThreadPool&) = delete;
    AsioThreadPool& operator=(const AsioThreadPool&) = delete;

    asio::thread_pool threadPool_ = boost::asio::thread_pool(std::thread::hardware_concurrency()) ;
};

/* This is an example of how to use the AsioThreadPool class
 * need to add this as a docygen example
class MyClass {
public:
    void myMemberFunction(int param1, double param2, const std::string& param3) {
        std::cout << "Parameters: " << param1 << ", " << param2 << ", " << param3 << std::endl;
    }
};

void myPostFunction(boost::asio::thread_pool& threadPool, const std::function<void()>& func) {
    boost::asio::post(threadPool, func);
}

int main() {
    boost::asio::thread_pool threadPool;

    MyClass myObject;
    int param1 = 42;
    double param2 = 3.14;
    std::string param3 = "Hello, world!";

    std::function<void(int, double, const std::string&)> boundFunc =
            std::bind(&MyClass::myMemberFunction, &myObject, param1, param2, param3);
    myPostFunction(threadPool, boundFunc);

    threadPool.join();

    return 0;
}
*/

#endif // ASIO_THREAD_POOL_HPP