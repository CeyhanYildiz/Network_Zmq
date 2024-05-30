#ifndef MOVIEMIXER_H
#define MOVIEMIXER_H

#include <iostream>
#include <string>
#include <fstream>
#include <zmq.hpp>
#include <filesystem>

using namespace zmq;
using namespace std;
namespace fs = std::filesystem;

class MovieMixer {
private:
    context_t context;
    socket_t subscriber;
    string databaseDirectory;

    void createGenreFiles(const string& userDirectory);

public:
    MovieMixer();
    void start();
};

#endif // MOVIEMIXER_H
