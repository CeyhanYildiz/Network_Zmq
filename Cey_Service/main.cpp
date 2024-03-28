#include <iostream>
#include <zmq.hpp>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#ifndef _WIN32
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

using namespace std;
using namespace zmq;

context_t context(1);

string get_random_joke(const string& joke_type) {
    string filename = "yourmum_" + joke_type + ".txt";
    ifstream file(filename);

    vector<string> jokes;
    string line;
    while (getline(file, line)) {
        jokes.push_back(line);
    }
    file.close();

    srand(time(0)); // Seed the random number generator
    int index = rand() % jokes.size();
    return jokes[index];
}

void recv_joke(const string& joke_type) {
    cout << joke_type << " jokes are Live" << endl;

    socket_t subscriber(context, ZMQ_SUB);
    socket_t ventilator(context, ZMQ_PUSH);

    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

    string subscribe_topic = "YourMum?>" + joke_type + ">";
    subscriber.set(sockopt::subscribe, subscribe_topic.c_str());

    while (true) {
        message_t msg;
        if (subscriber.recv(msg, recv_flags::none)) {
            string received_msg(static_cast<char*>(msg.data()), msg.size());
            cout << "Received: [" << received_msg << "]" << endl;

            string joke = get_random_joke(joke_type);
            string final_message = "YourMum!>" + joke_type + "> " + joke;

            message_t msgtosend(final_message.data(), final_message.size(), nullptr);
            ventilator.send(msgtosend, send_flags::none);

            // Add a delay of 1 second (adjust as needed)
            sleep(1);
        } else {
            // Handle receive failure if needed
        }
    }
}


int main() {
    vector<string> joke_types = {"Dum", "Ugly", "Fat" , "Random"};
    vector<thread> threads;

    for (const auto& joke_type : joke_types) {
        threads.emplace_back(recv_joke, joke_type);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
