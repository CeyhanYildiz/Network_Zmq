#include <iostream>
#include <zmq.hpp>
#include <thread>
#include <string>
#include <cstring> // for std::strlen

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

using namespace std;
using namespace zmq;

bool receivedMessage = false;

void pushMessage(const string& username, const string& password) {
    try {
        context_t context(1);
        socket_t ventilator(context, ZMQ_PUSH);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

        while (ventilator.handle() && !receivedMessage) {
            sleep(2000);
            string message = "PickMovieUsername?" + username + " " + password;
            message_t zmq_message(message.size());
            memcpy(zmq_message.data(), message.data(), message.size());
            ventilator.send(zmq_message, send_flags::none);
            cout << "Pushed: [" << message << "]" << endl;
        }
    }
    catch (error_t& ex) {
        cerr << "Caught an exception: " << ex.what();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <username> <password>" << endl;
        return 1;
    }

    thread t(pushMessage, argv[1], argv[2]);
    t.detach();

    try {
        context_t context(1);
        socket_t subscriber(context, ZMQ_SUB);
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "PickMovieUsername!", strlen("PickMovieUsername!"));

        while (subscriber.connected() && !receivedMessage) {
            message_t msg;
            subscriber.recv(&msg);
            cout << "Received: [" << string(static_cast<char*>(msg.data()), msg.size()) << "]" << endl;
            receivedMessage = true;
        }
    }
    catch (error_t& ex) {
        cerr << "Caught an exception: " << ex.what();
    }

    return 0;
}
