/** @file
    @brief Implementation

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>

*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include <osvr/Server/Server.h>
#include <osvr/Server/RegisterShutdownHandler.h>

// Library/third-party includes
// - none

// Standard includes
#include <iostream>
#include <exception>

using std::cout;
using std::cerr;
using std::endl;

static osvr::server::ServerPtr server;

/// @brief Shutdown handler function - forcing the server pointer to be global.
void handleShutdown() {
    cout << "Received shutdown signal..." << endl;
    server->signalStop();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Must supply at least one plugin name to load." << endl;
        return 1;
    }
    cout << "Creating server..." << endl;

    server = osvr::server::Server::createLocal();

    cout << "Registering shutdown handler..." << endl;
    osvr::server::registerShutdownHandler<&handleShutdown>();

    cout << "Loading plugins..." << endl;
    for (int i = 1; i < argc; ++i) {
        try {
            cout << "Loading plugin '" << argv[i] << "'..." << endl;
            server->loadPlugin(argv[i]);
            cout << "Plugin '" << argv[i] << "' loaded!\n" << endl;
        } catch (std::exception &e) {
            std::cerr << "Caught exception tring to load " << argv[i] << ": "
                      << e.what() << std::endl;
            return 1;
        }
    }

    cout << "Triggering a hardware detection..." << endl;
    server->triggerHardwareDetect();

    cout << "Starting server mainloop..." << endl;
    server->startAndAwaitShutdown();

    cout << "Server mainloop exited." << endl;

    return 0;
}