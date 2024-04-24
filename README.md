# Network_Zmq


## Setting up ZeroMQ (ZMQ) in a QT Project with MinGW

### Steps

1. **Copy Folders**:
   - Make a copy of the 'include' folder and the 'lib' folder provided in the github.
   - [include](https://github.com/CeyhanYildiz/Network_Zmq/include)
   - [lib](https://github.com/CeyhanYildiz/Network_Zmq/lib)

2. **Create a QT Project**:
   - Open QT Creator and create a new project.
   - Choose MinGW as the compiler toolchain.

3. **Modify the .pro File**:
   - Open the project's .pro file.
   - Add the following lines to the .pro file:
     ```pro
     TEMPLATE = app
     CONFIG += console c++11
     CONFIG -= app_bundle

     DEFINES += ZMQ_STATIC
     LIBS += -L$$PWD/../lib -lnzmqt -lzmq -lws2_32 -lIphlpapi
     INCLUDEPATH += $$PWD/../include

     SOURCES += main.cpp
     ```

4. **Test the Setup**:
   - Use ZMQbroker or write a simple test application to ensure the setup is correct.
   - Compile and run the project to verify that it works as expected.

### Notes
- `DEFINES += ZMQ_STATIC`: Defines the `ZMQ_STATIC` macro for static linking.
- `LIBS += -L$$PWD/../lib -lnzmqt -lzmq -lws2_32 -lIphlpapi`: Links against the necessary ZeroMQ libraries and Windows socket libraries.
- `INCLUDEPATH += $$PWD/../include`: Adds the path to the ZeroMQ headers.
