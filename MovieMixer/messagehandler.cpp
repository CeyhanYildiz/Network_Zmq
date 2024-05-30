#include "MessageHandler.h"

using namespace std;
using namespace zmq;

MessageHandler::MessageHandler() : context(1), subscriber(context, ZMQ_SUB), publisher(context, ZMQ_PUSH) {
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.set(sockopt::subscribe, "PickMovieUsername?");
    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
}

void MessageHandler::receiveMessages() {
    while (true) {
        message_t msg;
        if (!subscriber.recv(msg, recv_flags::none)) {
            cerr << "Failed to receive message" << endl;
            continue;
        }

        string message = string(static_cast<char*>(msg.data()), msg.size());
        cout << "Received: " << message << endl;

        string username = extractUsername(message);
        processMessage(username);
    }
}

string MessageHandler::extractUsername(const string& message) {
    return message.substr(message.find('>') + 1, message.find(' ') - message.find('>') - 1);
}

void MessageHandler::processMessage(const string& username) {
    if (userManager.userExists(username)) {
        sendResponse(username, "UsernameTaken");
    } else {
        userManager.createUser(username, "1234Koe1234");
        sendResponse(username, "created");
    }
}

void MessageHandler::sendResponse(const string& username, const string& status) {
    string response = "PickMovieUsername!>" + username + ">" + status;
    publisher.send(buffer(response), send_flags::none);
    cout << "Sent: " << response << endl;
}
