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

#include <vsomeip/vsomeip.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include<unistd.h>

std::shared_ptr< vsomeip::application > app;
#define SAMPLE_SERVICE_ID 0x1102
#define SAMPLE_INSTANCE_ID 0x0
#define SAMPLE_METHOD_ID 0x0102//0427

void on_request_message(const std::shared_ptr<vsomeip::message> &_request);

void runStart() {
    app->start();
}

void run() {
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_request_message);
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
}

//env VSOMEIP_CONFIGURATION=../vSomeip-local.json VSOMEIP_APPLICATION_NAME=vSomeIPServiceExample ./vSomeIPServiceExample
void on_request_message(const std::shared_ptr<vsomeip::message> &_request) {
    std::cout<<"Received on_request_message"<<std::endl;
    std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
    vsomeip::length_t l = its_payload->get_length();


    vsomeip::byte_t *response = (its_payload->get_data());
    // Get payload
    std::stringstream ss;
    for (vsomeip::length_t i=0; i<l; i++) {
       ss << std::setw(2) << std::setfill('0') << std::hex
          << (int)(response[i]) << " ";  //(int)*(its_payload->get_data())
    }

    std::cout << "SERVICE: Received message with Client/Session ["
        << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
        << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
        << ss.str() << std::endl;

    // Create response
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (int i=0; i<9; i++) {
        its_payload_data.push_back(i + 10);
    }
    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);
    std::cout<<"Sending response"<<std::endl;
    app->send(its_response);
}
int main() {
    app = vsomeip::runtime::get()->create_application("mE_rpcServer");
    app->init();
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_request_message);
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->start();
}
