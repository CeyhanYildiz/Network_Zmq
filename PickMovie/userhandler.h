#ifndef USER_HANDLER_H
#define USER_HANDLER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <map>
#include <zmq.hpp>

using namespace std;
using namespace zmq;

class UserHandler {
public:
    UserHandler(const  string& dbPath,  context_t& context);

    void run();

private:
     pair< string,  string> extractUsernameAndPassword(const  string& message);
    bool isUsernameExists(const  string& username);
    void createUserFolder(const  string& username, const  string& password);
    void createGenreFiles(const  filesystem::path& userDir);
     string maskUsername(const  string& username);

     string dbPath;
     context_t& context;
     socket_t subscriber;
     socket_t pusher;
};

#endif // USER_HANDLER_H
