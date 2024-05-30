#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <zmq.hpp>
#include <chrono>
#include <thread>
#include <unordered_set>

using namespace zmq;
using namespace std;
namespace fs = std::filesystem;
using namespace chrono_literals;

void handleUser(const string& username, const string& password, socket_t& subscriber, socket_t& pusher) {
    cout << "Handling user: " << username << endl;

    // Subscribe to the user with their username and password
    string topic = "PickMovie" + username + password + "?";
    subscriber.set(sockopt::subscribe, topic.c_str());

    // Push message for the user
    string pushMessage = username + "!";
    message_t msg(pushMessage.begin(), pushMessage.end());
    pusher.send(msg, zmq::send_flags::none);

    cout << "Subscribed to topic: " << topic << endl;

    // You can add your handling logic here
}

void receiveMessages(socket_t& subscriber, socket_t& pusher) {
    while (true) {
        message_t msg;
        if (subscriber.recv(msg, zmq::recv_flags::none)) {
            string received_msg(static_cast<char*>(msg.data()), msg.size());
            cout << "Received message: " << received_msg << endl;

            // Extract username and password from the received message
            string username = received_msg.substr(9, 17); // Assuming the username is always 17 characters starting from index 9
            string password = received_msg.substr(26, 12); // Assuming the password is always 12 characters starting from index 26

            // Create a reply message
            string replyMessage = "Replying to " + username + "'s message.";

            // Push the reply message to the user
            message_t reply(replyMessage.begin(), replyMessage.end());
            pusher.send(reply, zmq::send_flags::none);
        } else {
            cerr << "Error receiving message" << endl;
        }
    }
}



int main() {
    try {
        context_t context(1);

        // Setup subscribe connection for receiving messages from all users
        socket_t subscriber(context, ZMQ_SUB);
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24041");

        // Setup push connection for sending messages to all users
        socket_t pusher(context, ZMQ_PUSH);
        pusher.connect("tcp://benternet.pxl-ea-ict.be:24042");

        // Setup request connection for checking user live status
        socket_t requester(context, ZMQ_REQ);
        requester.connect("tcp://benternet.pxl-ea-ict.be:24041");

        string databaseDirectory = "C:/Users/Cey/Documents/PXL_23-24/S2 Netwerk/Network_Zmq/build-MovieMixer-Desktop_Qt_6_6_1_MinGW_64_bit-Debug/Database";

        // Set to store usernames found during each scan
        unordered_set<string> previousUsernames;

        // Start a thread to receive messages
        thread receiveThread(receiveMessages, ref(subscriber), ref(pusher));

        while (true) {
            // Print a message indicating that it's scanning
            //cout << "Scanning for new users..." << endl;

            // Iterate through each directory in the database
            for (const auto& entry : fs::directory_iterator(databaseDirectory)) {
                if (entry.is_directory()) {
                    string username = entry.path().filename().string();

                    // Check if the username is new
                    if (previousUsernames.find(username) == previousUsernames.end()) {
                        // Read password from password.txt
                        string password;
                        ifstream passwordFile(entry.path() / "password.txt");
                        if (passwordFile.is_open()) {
                            getline(passwordFile, password);
                            passwordFile.close();
                        } else {
                            cout << "Error: Unable to open password file for user: " << username << endl;
                            continue; // Skip handling this user
                        }

                        // New username found, handle it in a separate thread
                        thread handleThread(handleUser, username, password, ref(subscriber), ref(pusher));
                        handleThread.detach(); // Detach the thread to run independently
                        cout << "New user found: " << username << endl;
                        previousUsernames.insert(username); // Add username to the set
                    }
                }
            }

            // Wait for a short duration before scanning again
            this_thread::sleep_for(5s);
        }
    } catch (error_t & ex) {
        cerr << "Caught an exception: " << ex.what() << endl;
    }

    return 0;
}
