#include "MovieMixer.h"

void MovieMixer::createGenreFiles(const string& userDirectory) {
    string genres[] = {
        "Action", "Adventure", "Animation", "Comedy", "Crime", "Drama",
        "Fantasy", "Historical", "Horror", "Musical", "Mystery", "Romance",
        "Science Fiction", "Thriller", "War", "Documentary", "Family", "Sports"
    };

    for (const auto& genre : genres) {
        ofstream genreFile(userDirectory + "/" + genre + ".txt");
        genreFile.close();
    }
}

MovieMixer::MovieMixer() : context(1), subscriber(context, ZMQ_SUB) {
    databaseDirectory = "C:/Users/Cey/Documents/PXL_23-24/S2 Netwerk/Network_Zmq/build-MovieMixer-Desktop_Qt_6_6_1_MinGW_64_bit-Debug/Database";
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.set(sockopt::subscribe, "PickMovieUsername?");
    if (!fs::exists(databaseDirectory)) {
        fs::create_directory(databaseDirectory);
    }
}

void MovieMixer::start() {
    try {
        while (true) {
            message_t msg;
            if (!subscriber.recv(msg, recv_flags::none)) {
                cerr << "Failed to receive message" << endl;
                continue;
            }

            string message = string(static_cast<char*>(msg.data()), msg.size());
            cout << "Received: " << message << endl;

            string username = message.substr(message.find('>') + 1, message.find(' ') - message.find('>') - 1);
            string userDirectory = databaseDirectory + "/" + username;

            if (fs::exists(userDirectory)) {
                socket_t publisher(context, ZMQ_PUSH);
                publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
                string response = "PickMovieUsername!>" + username + ">UsernameTaken";
                publisher.send(buffer(response), send_flags::none);
                cout << "Sent: " << response << endl;
            } else {
                fs::create_directory(userDirectory);
                ofstream passwordFile(userDirectory + "/password.txt");
                string password = "1234Koe1234"; // Example password
                passwordFile << password;
                passwordFile.close();

                createGenreFiles(userDirectory);

                socket_t publisher(context, ZMQ_PUSH);
                publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
                string response = "PickMovieUsername!>" + username + ">created";
                publisher.send(buffer(response), send_flags::none);
                cout << "Sent: " << response << endl;
            }
        }
    } catch (error_t & ex) {
        cerr << "Caught an exception: " << ex.what() << endl;
    }
}
