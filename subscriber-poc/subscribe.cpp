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
#include "subscribe.h";


int main(int argc, char** argv) {
	std::string url("amqp://localhost:61616");
	std::string address("topic.foo");
	int desired = 1;
	int received = 0;

	subscribe sub(url, address, "Label One", desired, received);
	subscribe subTwo(url, address, "Label Two", desired, received);
	subscribe subThree(url, address, "Label Three", desired, received);

	proton::container cont{ sub };
	//proton::container contTwo{ subTwo };
	//proton::container contThree{ subThree };

	try {
		cont.run();
		//contTwo.run();
		//contThree.run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << "\n";
		return 1;
	}

	return 0;
}
