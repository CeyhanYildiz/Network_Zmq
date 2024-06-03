#include "UserHandler.h"
#include <iostream>
#include <thread>
#include <filesystem>


using namespace std;
using namespace zmq;

UserHandler::UserHandler(const  string& dbPath,  context_t& context) // auto set connaction to benternet
    : dbPath(dbPath), context(context), subscriber(context, ZMQ_SUB), pusher(context, ZMQ_PUSH) {
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.set( sockopt::subscribe, "PickMovieUsername?");
    pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");
}

void UserHandler::run() {
    while (true) {
         message_t msg;
        subscriber.recv(msg,  recv_flags::none);
         string message(static_cast<char*>(msg.data()), msg.size()); // msg form Login_Client
         cout << "Received: " << message <<  endl;
        auto [username, password] = extractUsernameAndPassword(message); // Get username and passord
        if (!username.empty() && !password.empty()) {
            if (!isUsernameExists(username)) { // Check if exists
                createUserFolder(username, password); // Make new usernaem folder
                 string response = "PickMovieUsername!" + maskUsername(username) + ":created";
                pusher.send( buffer(response),  send_flags::dontwait); // Send msg to Login_Client
                 cout << "Pushing: " << response <<  endl;
            } else {
                 cout << "Username already exists: " << username <<  endl;
            }
        }
    }
}

 pair< string,  string> UserHandler::extractUsernameAndPassword(const  string& message) {
    size_t startPos = message.find('?');
    if (startPos !=  string::npos) {
         string username = message.substr(startPos + 1, message.find(' ', startPos + 1) - startPos - 1);
         string password = message.substr(message.find(' ', startPos + 1) + 1);
        return  make_pair(username, password);
    }
    return  make_pair("", "");
}

bool UserHandler::isUsernameExists(const  string& username) {
     filesystem::path userDir = dbPath + "\\" + username;
    if ( filesystem::exists(userDir)) {
         string response = "PickMovieUsername!" + maskUsername(username) + " is already taken.";
        pusher.send( buffer(response),  send_flags::dontwait);
        return true;
    }
    return false;
}

void UserHandler::createUserFolder(const  string& username, const  string& password) {
     filesystem::path userDir = dbPath + "\\" + username;
     filesystem::create_directory(userDir);
     ofstream passwordFile(userDir / "password.txt");
    if (passwordFile.is_open()) {
        passwordFile << password;
        passwordFile.close();
    }
    createGenreFiles(userDir);
}

void UserHandler::createGenreFiles(const  filesystem::path& userDir) {
     map<int,  string> genres = {
        {1, "Action"}, {2, "Adventure"}, {3, "Animation"}, {4, "Comedy"}, {5, "Crime"},
        {6, "Drama"}, {7, "Fantasy"}, {8, "Historical"}, {9, "Horror"}, {10, "Musical"},
        {11, "Mystery"}, {12, "Romance"}, {13, "Science Fiction"}, {14, "Thriller"}, {15, "War"},
        {16, "Documentary"}, {17, "Family"}, {18, "Sports"}
    };
    for (const auto& [index, genre] : genres) {
         ofstream genreFile(userDir / (genre + ".txt"));
        genreFile.close();
    }
}

 string UserHandler::maskUsername(const  string& username) {
    return username.substr(0, 1) + "******" + username.substr(username.size() - 2);
}
