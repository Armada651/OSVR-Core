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

#define OSVR_DEV_VERBOSE_DISABLE

// Internal Includes
#include "AsyncDeviceToken.h"
#include <osvr/Connection/ConnectionDevice.h>

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
// - none

namespace osvr {
namespace connection {
    using boost::unique_lock;
    using boost::mutex;

    AsyncDeviceToken::AsyncDeviceToken(std::string const &name)
        : DeviceToken(name) {}

    AsyncDeviceToken::~AsyncDeviceToken() {
        OSVR_DEV_VERBOSE("AsyncDeviceToken\t"
                         "In ~AsyncDeviceToken");

        signalAndWaitForShutdown();
    }

    void AsyncDeviceToken::signalShutdown() {
        OSVR_DEV_VERBOSE("AsyncDeviceToken\t"
                         "In signalShutdown");
        m_run.signalShutdown();
        m_accessControl.mainThreadDenyPermanently();
    }

    void AsyncDeviceToken::signalAndWaitForShutdown() {
        OSVR_DEV_VERBOSE("AsyncDeviceToken\t"
                         "In signalAndWaitForShutdown");
        signalShutdown();
        m_run.signalAndWaitForShutdown();
        m_callbackThread.join();
    }

    namespace {
        /// @brief Function object for the wait callback loop of an
        /// AsyncDeviceToken
        class WaitCallbackLoop {
          public:
            WaitCallbackLoop(::util::RunLoopManagerBase &run,
                             AsyncDeviceWaitCallback const &cb)
                : m_cb(cb), m_run(&run) {}
            void operator()() {
                OSVR_DEV_VERBOSE("WaitCallbackLoop starting");
                ::util::LoopGuard guard(*m_run);
                while (m_run->shouldContinue()) {
                    m_cb();
                }
                OSVR_DEV_VERBOSE("WaitCallbackLoop exiting");
            }

          private:
            AsyncDeviceWaitCallback m_cb;
            ::util::RunLoopManagerBase *m_run;
        };
    } // end of anonymous namespace

    void AsyncDeviceToken::setWaitCallback(AsyncDeviceWaitCallback const &cb) {
        m_callbackThread = boost::thread(WaitCallbackLoop(m_run, cb));
        m_run.signalAndWaitForStart();
    }

    AsyncDeviceToken *AsyncDeviceToken::asAsync() { return this; }

    void AsyncDeviceToken::m_sendData(util::time::TimeValue const &timestamp,
                                      MessageType *type, const char *bytestream,
                                      size_t len) {
        OSVR_DEV_VERBOSE("AsyncDeviceToken::m_sendData\t"
                         "about to create RTS object");
        RequestToSend rts(m_accessControl);

        bool clear = rts.request();
        if (!clear) {
            OSVR_DEV_VERBOSE("AsyncDeviceToken::m_sendData\t"
                             "RTS request responded with not clear to send.");
            return;
        }

        OSVR_DEV_VERBOSE("AsyncDeviceToken::m_sendData\t"
                         "Have CTS!");
        m_getConnectionDevice()->sendData(timestamp, type, bytestream, len);
        OSVR_DEV_VERBOSE("AsyncDeviceToken::m_sendData\t"
                         "done!");
    }

    void AsyncDeviceToken::m_connectionInteract() {
        OSVR_DEV_VERBOSE("AsyncDeviceToken::m_connectionInteract\t"
                         "Going to send a CTS if waiting");
        bool handled = m_accessControl.mainThreadCTS();
        if (handled) {
            OSVR_DEV_VERBOSE("AsyncDeviceToken::m_connectionInteract\t"
                             "Handled an RTS!");
        } else {
            OSVR_DEV_VERBOSE("AsyncDeviceToken::m_connectionInteract\t"
                             "No waiting RTS!");
        }
    }

    void AsyncDeviceToken::m_stopThreads() { signalAndWaitForShutdown(); }

} // namespace connection
} // namespace osvr
