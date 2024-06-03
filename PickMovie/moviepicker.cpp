#include <fstream>
#include "MoviePicker.h"
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <algorithm>

using namespace std;
using namespace zmq;

MoviePicker::MoviePicker(const string& moviePath, context_t& context)
    : moviePath(moviePath), context(context), subscriber(context, ZMQ_SUB), pusher(context, ZMQ_PUSH) {
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.set(sockopt::subscribe, "PickMovie?");
    pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    initGenreFiles();
}

void MoviePicker::run() {
    srand(static_cast<unsigned>(time(nullptr)));
    while (true) {
        message_t msg;
        subscriber.recv(msg, recv_flags::none); // Get msg from PickMovie_Client
        string message(static_cast<char*>(msg.data()), msg.size());
        cout << "Received: " << message << endl;

        string movieGenre = extractMovieGenre(message); // Extract Movie Genre
        int movieCount = extractNumberOfMovies(message); // Extract number of movies

        if (!movieGenre.empty() && movieGenre != "Genres") {
            auto it = genreFiles.find(movieGenre);
            if (it != genreFiles.end()) {
                vector<string> pickedMovies = pickRandomMovies(it->second, movieCount); // Pick random movies
                if (!pickedMovies.empty()) {
                    string response = "PickMovie! Genre: " + movieGenre + ". Picked Movies: ";
                    for (const auto& movie : pickedMovies) {
                        response += movie + ", ";
                    }
                    response = response.substr(0, response.size() - 2); // Remove trailing comma and space
                    pusher.send(buffer(response), send_flags::dontwait);
                    cout << "Pushing: " << response << endl;
                } else {
                    cerr << "No movies found for genre: " << movieGenre << endl;
                }
            } else {
                cerr << "Genre not found: " << movieGenre << endl;
            }
        } else if (movieGenre == "Genres") {
            vector<string> pickedGenres = pickRandomGenres(movieCount); // Pick random genres
            if (!pickedGenres.empty()) {
                string response = "PickMovie! Picked Genres: ";
                for (const auto& genre : pickedGenres) {
                    response += genre + ", ";
                }
                response = response.substr(0, response.size() - 2); // Remove trailing comma and space
                pusher.send(buffer(response), send_flags::dontwait);
                cout << "Pushing: " << response << endl;
            } else {
                cerr << "No genres found" << endl;
            }
        }
    }
}

void MoviePicker::initGenreFiles() {
    genreFiles = {
        {"Action", "Action.csv"}, {"Adventure", "Adventure.csv"}, {"Animation", "Animation.csv"},
        {"Comedy", "Comedy.csv"}, {"Crime", "Crime.csv"}, {"Documentary", "Documentary.csv"},
        {"Drama", "Drama.csv"}, {"Family", "Family.csv"}, {"Fantasy", "Fantasy.csv"},
        {"Historical", "Historical.csv"}, {"Horror", "Horror.csv"}, {"Musical", "Musical.csv"},
        {"Mystery", "Mystery.csv"}, {"Romance", "Romance.csv"}, {"Sci-Fi", "Sci-Fi.csv"},
        {"Sports", "Sports.csv"}, {"Thriller", "Thriller.csv"}, {"War", "War.csv"}
    };
}

string MoviePicker::extractMovieGenre(const string& message) {
    istringstream iss(message);
    string word;
    for (int i = 0; i < 3; ++i) {
        if (!(iss >> word)) return "";
    }
    return word;
}

int MoviePicker::extractNumberOfMovies(const string& message) {
    istringstream iss(message);
    string word;
    for (int i = 0; i < 4; ++i) {
        if (!(iss >> word)) return 1; // Default to 1 if not provided
    }
    try {
        return stoi(word);
    } catch (const exception&) {
        return 1; // Default to 1 if parsing fails
    }
}

string MoviePicker::pickRandomMovie(const string& genreFile) {
    ifstream file(moviePath + "\\" + genreFile);
    string line;
    vector<string> movies;
    if (file.is_open()) {
        while (getline(file, line)) {
            movies.push_back(line);
        }
        file.close();
    }
    if (!movies.empty()) {
        int randomIndex = rand() % movies.size();
        string pickedMovie = movies[randomIndex];
        replace(pickedMovie.begin(), pickedMovie.end(), ';', ' ');
        return pickedMovie;
    }
    return "";
}

vector<string> MoviePicker::pickRandomMovies(const string& genreFile, int count) {
    ifstream file(moviePath + "\\" + genreFile);
    string line;
    vector<string> movies;
    if (file.is_open()) {
        while (getline(file, line)) {
            movies.push_back(line);
        }
        file.close();
    }

    vector<string> pickedMovies;
    unordered_set<int> pickedIndices;
    while (pickedMovies.size() < count && pickedIndices.size() < movies.size()) {
        int index = rand() % movies.size();
        if (pickedIndices.insert(index).second) {
            string pickedMovie = movies[index];
            replace(pickedMovie.begin(), pickedMovie.end(), ';', ' ');
            pickedMovies.push_back(pickedMovie);
        }
    }
    return pickedMovies;
}

vector<string> MoviePicker::pickRandomGenres(int count) {
    vector<string> allGenres;
    for (const auto& pair : genreFiles) {
        allGenres.push_back(pair.first);
    }

    vector<string> pickedGenres;
    unordered_set<int> pickedIndices;
    while (pickedGenres.size() < count && pickedIndices.size() < allGenres.size()) {
        int index = rand() % allGenres.size();
        if (pickedIndices.insert(index).second) {
            pickedGenres.push_back(allGenres[index]);
        }
    }
    return pickedGenres;
}
