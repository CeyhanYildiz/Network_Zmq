#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <string>
#include <zmq.hpp>
#include "UserManager.h"

using namespace std;
using namespace zmq;

class MessageHandler {
public:
    MessageHandler();
    void receiveMessages();

private:
    string extractUsername(const string& message);
    void processMessage(const string& username);
    void sendResponse(const string& username, const string& status);

    context_t context;
    socket_t subscriber;
    socket_t publisher;
    UserManager userManager;
};

#endif /* MESSAGEHANDLER_H */
