#include <iostream>
#include <zmq.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

using namespace zmq;
using namespace std;
using namespace chrono;

atomic<bool> stopSending(false); // Atomic flag to stop sending messages

void sendMessage(socket_t& socket, const string& message, int time) {
    while (!stopSending) {
        socket.send(const_buffer(message.data(), message.size()), send_flags::none);
        cout << "Sent: [" << message << "]" << endl;
        this_thread::sleep_for(milliseconds(time)); // Sleep for the specified time (in milliseconds)
    }
}

void receiveMessage(socket_t& socket, const string& filter) {
    while (!stopSending) {
        message_t msg;
        if (socket.recv(msg)) {
            string received_msg(static_cast<char*>(msg.data()), msg.size());
            if (received_msg.find(filter) != string::npos) {
                cout << "Received: [" << received_msg << "]" << endl;
                stopSending = true; // Stop sending when the first message is received
            }
        }
    }
}

int main() {
    try {
        context_t context(1);

        // Define endpoints
        vector<string> endpoints = {
            "tcp://benternet.pxl-ea-ict.be:24041",
            "tcp://benternet.pxl-ea-ict.be:24042"
            // Add more endpoints if needed
        };

        // Define username and password
        string username = "KingKong007";
        string password = "1234Koe1234";

        // Create and connect sockets for each endpoint
        vector<socket_t> ventilators;
        vector<socket_t> subscribers;
        for (const auto& endpoint : endpoints) {
            socket_t ventilator(context, ZMQ_PUSH);
            ventilator.connect(endpoint);
            ventilators.push_back(move(ventilator));

            socket_t subscriber(context, ZMQ_SUB);
            subscriber.connect(endpoint);
            // Subscribe to the specific topic
            subscriber.set(sockopt::subscribe, "PickMovieKingKong0071234Koe1234!");
            subscribers.push_back(move(subscriber));
        }

        // Threads for receiving messages for each endpoint
        vector<thread> receiveThreads;
        for (size_t i = 0; i < endpoints.size(); ++i) {
            receiveThreads.emplace_back(receiveMessage, ref(subscribers[i]), "PickMovieKingKong0071234Koe1234!");
        }

        // Create a single thread for sending messages
        string message = "PickMovieKingKong0071234Koe1234?Horror"; // Change the message content as needed
        thread sendThread(sendMessage, ref(ventilators[0]), message, 2000); // Send every 2 second

        // Join sendThread to prevent the main function from exiting
        sendThread.join();

        // Join receiveThreads to ensure they complete before exiting
        for (auto& t : receiveThreads) {
            t.join();
        }
    } catch (error_t & ex) {
        cerr << "Caught an exception: " << ex.what() << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
