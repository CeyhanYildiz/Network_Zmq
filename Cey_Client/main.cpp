#include <iostream>
#include <zmq.hpp>
#include <thread> // Include for threads
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

using namespace std;
using namespace zmq;

void sendMessage(socket_t& socket, const char* message, size_t size, int time) {
    while (true) {
        socket.send(const_buffer(message, size), send_flags::none);
        cout << "Pushed: [" << message << "]" << endl;
        sleep(time*1000);
    }
}

void receiveMessage(socket_t& socket, const char* filter) {
    while (true) {
        message_t msg;
        if (socket.recv(msg)) {
            string received_msg(static_cast<char*>(msg.data()), msg.size());
            if (received_msg.find(filter) == 0) {
                cout << "Received: [" << received_msg << "]" << endl;
            }
        }
    }
}

int main() {
    context_t context(1);
    socket_t ventilator(context, ZMQ_PUSH);
    socket_t subscriber(context, ZMQ_SUB);

    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");

    // Subscribe to all topics
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    // // Threads for sending and receiving messages
    thread t1(sendMessage, ref(ventilator), "YourMum?>Dum>",    14, 5);
    thread t2(receiveMessage, ref(subscriber), "YourMum!>Dum>");

    // // Threads for sending and receiving messages
    // thread t3(sendMessage, ref(ventilator), "YourMum?>Fat>",    14, 5);
    // thread t4(receiveMessage, ref(subscriber), "YourMum!>Fat>");


    // // Threads for sending and receiving messages
    // thread t5(sendMessage, ref(ventilator), "YourMum?>Ugly>",    15, 5);
    // thread t6(receiveMessage, ref(subscriber), "YourMum!>Ugly>");

    // Threads for sending and receiving messages
    //thread t7(sendMessage, ref(ventilator), "YourMum?>Random>",    17, 5);
    //thread t8(receiveMessage, ref(subscriber), "YourMum!>Random>");


    // // Join threads to prevent the main function from exiting
    t1.join();
    t2.join();

    // // Join threads to prevent the main function from exiting
    // t3.join();
    // t4.join();

    // // Join threads to prevent the main function from exiting
    // t5.join();
    // t6.join();

    // Join threads to prevent the main function from exiting
    //t7.join();
    //t8.join();



    return 0;
}
