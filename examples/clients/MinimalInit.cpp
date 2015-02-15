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
#include <osvr/ClientKit/ClientKit.h>

// Library/third-party includes
// - none

// Standard includes
#include <iostream>

int main() {
    osvr::clientkit::ClientContext context(
        "org.opengoggles.exampleclients.MinimalInit");
    std::cout << "OK, library initialized." << std::endl;

    // Pretend that this is your application's mainloop.
    for (int i = 0; i < 1000000; ++i) {
        context.update();
    }

    std::cout << "Library shut down, exiting." << std::endl;
    return 0;
}
