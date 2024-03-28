#include <iostream>
#include <zmq.hpp>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

using namespace std;
using namespace zmq;

int main() {
    context_t context(1);
    socket_t ventilator(context, ZMQ_PUSH);

    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

    while (ventilator.handle() != nullptr) {
        sleep(2000);
        ventilator.send(const_buffer("YourMum?>Dum>", 14), send_flags::none);
        cout << "Pushed : [YourMum?>Dum]" << endl;

        sleep(2000);
        ventilator.send(const_buffer("YourMum?>Fat>", 14), send_flags::none);
        cout << "Pushed : [YourMum?>Fat]" << endl;

        sleep(2000);
        ventilator.send(const_buffer("YourMum?>Random>", 17), send_flags::none);
        cout << "Pushed : [YourMum?>Random>]" << endl;

        sleep(2000);
        ventilator.send(const_buffer("YourMum?>Ugly>", 15), send_flags::none);
        cout << "Pushed : [YourMum?>Ugly>]" << endl;
    }

    return 0;
}
