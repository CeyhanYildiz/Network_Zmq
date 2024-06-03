#include <string>
#include <thread>
#include <filesystem>
#include <zmq.hpp>
#include "UserHandler.h"
#include "MoviePicker.h"



using namespace std;
using namespace zmq;

int main() {
    context_t context(1);

    UserHandler userHandler("C:\\Users\\Cey\\Desktop\\Database", context);
    MoviePicker moviePicker("C:\\Users\\Cey\\Desktop\\Movie\\Movies", context);

     thread handlerThread(&UserHandler::run, &userHandler);
     thread pickMovieHandlerThread(&MoviePicker::run, &moviePicker);

    handlerThread.join();
    pickMovieHandlerThread.join();

    return 0;
}
