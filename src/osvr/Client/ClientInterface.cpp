/** @file
    @brief Implementation

    @date 2014

    @author
    Ryan Pavlik
    <ryan@sensics.com>
    <http://sensics.com>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include <osvr/Client/ClientInterface.h>
#if !defined(__ANDROID__)
#include <osvr/Util/Verbosity.h>
#else
#include <osvr/Util/MacroToolsC.h>
#if defined(OSVR_UTIL_DEV_VERBOSE) && !defined(OSVR_DEV_VERBOSE_DISABLE)
#include <iostream>
#define OSVR_DEV_VERBOSE(X)                                                    \
    OSVR_UTIL_MULTILINE_BEGIN std::cout << "[OSVR] " << X << std::endl;        \
    OSVR_UTIL_MULTILINE_END
#else
#define OSVR_DEV_VERBOSE(X) OSVR_UTIL_MULTILINE_BEGIN OSVR_UTIL_MULTILINE_END
#endif
#endif

// Library/third-party includes
// - none

// Standard includes
#include <boost/range/algorithm.hpp>

OSVR_ClientInterfaceObject::OSVR_ClientInterfaceObject(
    ::osvr::client::ClientContext *ctx, std::string const &path,
    OSVR_ClientInterfaceObject::PrivateConstructor const &)
    : m_ctx(ctx), m_path(path) {
    OSVR_DEV_VERBOSE("Interface initialized for " << m_path);
}

std::string const &OSVR_ClientInterfaceObject::getPath() const {
    return m_path;
}

::osvr::client::ClientContext &OSVR_ClientInterfaceObject::getContext() {
    return *m_ctx;
}

#define OSVR_CALLBACK_METHODS(TYPE)                                            \
    void OSVR_ClientInterfaceObject::registerCallback(                         \
        OSVR_##TYPE##Callback cb, void *userdata) {                            \
        using namespace std::placeholders;                                     \
        m_callbacks##TYPE.push_back(std::bind(cb, userdata, _1, _2));          \
    }                                                                          \
    void OSVR_ClientInterfaceObject::triggerCallbacks(                         \
        const OSVR_TimeValue &timestamp, const OSVR_##TYPE##Report &report) {  \
        for (auto const &f : m_callbacks##TYPE) {                              \
            f(&timestamp, &report);                                            \
        }                                                                      \
    }

OSVR_CALLBACK_METHODS(Pose)
OSVR_CALLBACK_METHODS(Position)
OSVR_CALLBACK_METHODS(Orientation)
OSVR_CALLBACK_METHODS(Button)
OSVR_CALLBACK_METHODS(Analog)
#undef OSVR_CALLBACK_METHODS

void OSVR_ClientInterfaceObject::update() {}
