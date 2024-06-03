#ifndef MOVIE_PICKER_H
#define MOVIE_PICKER_H

#include <string>
#include <map>
#include <vector>
#include <zmq.hpp>

using namespace std;
using namespace zmq;

class MoviePicker {
public:
    MoviePicker(const string& moviePath, context_t& context);

    void run();

private:
    void initGenreFiles();
    string extractMovieGenre(const string& message);
    int extractNumberOfMovies(const string& message);
    string pickRandomMovie(const string& genreFile);
    vector<string> pickRandomMovies(const string& genreFile, int count);
    vector<string> pickRandomGenres(int count);

    string moviePath;
    context_t& context;
    socket_t subscriber;
    socket_t pusher;
    map<string, string> genreFiles;
};

#endif // MOVIE_PICKER_H
