#include <proton/connection.hpp>
#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/receiver.hpp>
#include <proton/sender.hpp>
#include <proton/work_queue.hpp>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include "subscribe.h"


int main(int argc, char** argv) {
	std::string url("amqp://localhost:61616");
	std::string address("topic.valorEconomico");

	subscribe sub(url, address);

	proton::container cont{ sub };

	try {
		cont.run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << "\n";
		return 1;
	}

	return 0;
}
