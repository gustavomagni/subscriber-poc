/*
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
 */

#include "options.h"

#include <proton/connection.hpp>
#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/link.hpp>
#include <proton/message.hpp>
#include <proton/message_id.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/value.hpp>
#include <proton/receiver_options.hpp>
#include <proton/source_options.hpp>
#include <proton/target_options.hpp>

#include <iostream>
#include <map>

#include "fake_cpp11.h"

using proton::receiver_options;
using proton::source_options;
using proton::target_options;

// Lock output from threads to avoid scrambling
std::mutex out_lock;
#define OUT(x) do { std::lock_guard<std::mutex> l(out_lock); x; } while (false)

#pragma once
class subscribe : public proton::messaging_handler
{
private:
	std::string conn_url_{};
	std::string address_{};
	std::string label_{};
	int desired_{ 0 };
	int received_{ 0 };

public:
	subscribe(const std::string& conn, const std::string& address, const std::string& label, const int& desired, const int& received) : conn_url_(conn), address_(address), label_(label), desired_(desired), received_(received) {}

	void on_container_start(proton::container& cont) override {
		cont.connect(conn_url_);
	}

	void on_connection_open(proton::connection& conn) override {
		source_options source_options;
		source_options.dynamic(false);
		source_options.address("topic.valorEconomico");

		receiver_options receiver_options;
		receiver_options.source(source_options);

		conn.open_receiver(address_, receiver_options);
	}

	void on_receiver_open(proton::receiver& rcv) override {
		std::cout << "RECEIVE: Opened receiver for source address '" << rcv.source().address() << "' Id: '" << label_ << "'\n";
	}

	void on_message(proton::delivery& dlv, proton::message& msg) override {
		std::cout << "RECEIVE: Received message '" << msg.body() << "'\n";

		received_++;

		//if (received_ == desired_) {
			//dlv.receiver().close();
			//dlv.connection().close();
		//}
	}
};

