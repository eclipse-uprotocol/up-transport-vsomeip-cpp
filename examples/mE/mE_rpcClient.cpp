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

#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1103
#define SAMPLE_INSTANCE_ID 0x0
#define SAMPLE_METHOD_ID 0x0100 //0x421

std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;

void run() {
  std::unique_lock<std::mutex> its_lock(mutex);
  condition.wait(its_lock);

  std::cout<<"Inside run() method"<<std::endl;

  std::shared_ptr< vsomeip::message > request;
  request = vsomeip::runtime::get()->create_request();
  request->set_service(SAMPLE_SERVICE_ID);
  request->set_instance(SAMPLE_INSTANCE_ID);
  request->set_method(SAMPLE_METHOD_ID);

  std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
  std::vector< vsomeip::byte_t > its_payload_data;
  for (vsomeip::byte_t i=0; i<10; i++) {
      its_payload_data.push_back(i);//i % 256
  }
  its_payload->set_data(its_payload_data);
  request->set_payload(its_payload);
  std::cout<<"Calling rpc method"<<std::endl;
  app->send(request);
}

void on_response_message(const std::shared_ptr<vsomeip::message> &_response) {
  std::cout<<"on_response_message()"<<std::endl;

  std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
  vsomeip::length_t l = its_payload->get_length();

  std::cout<<"Payload length is "<<l<<std::endl;
  // Get payload
  std::stringstream ss;
  /*
    for (vsomeip::length_t i=0; i<=l; i++) {
     ss << std::setw(2) << std::setfill('0') << std::hex
        << (int)*(its_payload->get_data()+i) << " ";
  }
  */
  for (vsomeip::length_t i=0; i<l; i++) {
     ss << (char)*(its_payload->get_data() + i) << " ";
  }

  /*
    std::cout << "CLIENT: Received message with Client/Session ["
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_client() << "/"
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_session() << "] "
      << ss.str() << std::endl;
  */
    std::cout << "CLIENT: Received message from uE : "<< ss.str() << std::endl;
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "CLIENT: Service ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << std::endl;

    if(_is_available)
        condition.notify_one();
}

int main() {

    app = vsomeip::runtime::get()->create_application("mE_rpcClient");
    std::cout<<"Create application 'mE_rpcClient' success"<<std::endl;
    app->init();
    std::cout<<"init() method success"<<std::endl;
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    std::cout<<"register_availability_handler success"<<std::endl;
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    std::cout<<"request_service success"<<std::endl;
    app->register_message_handler(SAMPLE_SERVICE_ID, 0xFFFF, 0xFFFF, on_response_message);//SAMPLE_INSTANCE_ID , SAMPLE_METHOD_ID
    std::cout<<"register_message_handler on method success"<<std::endl;
    std::thread sender(run);
    app->start();
    std::cout<<"vsomeip client has started ..."<<std::endl;
}