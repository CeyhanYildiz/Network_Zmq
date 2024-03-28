#include <iostream>
#include <zmq.hpp>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

int main( void )
{

    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );

    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );

    while( ventilator.connected() )
    {
        sleep( 2000);
        ventilator.send( "YourMum?>", 10 );
        std::cout << "Pushed : [YourMum?>]" << std::endl;

    }

    return 0;
}
