#include <iostream>
#include <zmq.hpp>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

zmq::context_t context(1);

std::string get_random_mum_joke() {
    std::ifstream file("yourmum.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file yourmum.txt" << std::endl;
        return "No jokes available";
    }

    std::vector<std::string> jokes;
    std::string line;
    while (std::getline(file, line)) {
        jokes.push_back(line);
    }
    file.close();

    if (jokes.empty()) {
        std::cerr << "Error: No jokes found in the file" << std::endl;
        return "No jokes available";
    }

    srand(time(0)); // Seed the random number generator
    int index = rand() % jokes.size();
    return jokes[index];
}

void recv_yourmum() {
    std::cout << "TEST_yourmum" << std::endl;

    zmq::socket_t subscriber_yourmum(context, ZMQ_SUB);
    zmq::socket_t ventilator_yourmum(context, ZMQ_PUSH);

    subscriber_yourmum.connect("tcp://benternet.pxl-ea-ict.be:24042");
    ventilator_yourmum.connect("tcp://benternet.pxl-ea-ict.be:24041");

    subscriber_yourmum.setsockopt(ZMQ_SUBSCRIBE, "YourMum?>", 10);

    while (true) {
        zmq::message_t msgr;
        subscriber_yourmum.recv(&msgr);

        std::string received_msg(static_cast<char*>(msgr.data()), msgr.size());
        std::cout << "Received: [" << received_msg << "]" << std::endl;

        std::string joke = get_random_mum_joke();
        std::string final_message = "YourMum!> " + joke;

        zmq::message_t msgtosend(final_message.size());
        memcpy(msgtosend.data(), final_message.data(), final_message.size());
        ventilator_yourmum.send(msgtosend);

        // Add a delay of 1 second (adjust as needed)
        sleep(1);
    }
}

int main() {
    std::thread first(recv_yourmum);
    first.join();

    return 0;
}
