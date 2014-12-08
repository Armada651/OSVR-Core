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
#include "SyncDeviceToken.h"
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

    SyncDeviceToken::SyncDeviceToken(std::string const &name)
        : DeviceToken(name) {}

    SyncDeviceToken::~SyncDeviceToken() {}

    void
    SyncDeviceToken::setUpdateCallback(SyncDeviceUpdateCallback const &cb) {
        OSVR_DEV_VERBOSE("In SyncDeviceToken::setUpdateCallback");
        m_cb = cb;
    }

    void SyncDeviceToken::m_sendData(util::time::TimeValue const &timestamp,
                                     MessageType *type, const char *bytestream,
                                     size_t len) {
        m_getConnectionDevice()->sendData(timestamp, type, bytestream, len);
    }

    void SyncDeviceToken::m_connectionInteract() {
        if (m_cb) {
            m_cb();
        }
    }

    SyncDeviceToken *SyncDeviceToken::asSync() { return this; }

} // namespace connection
} // namespace osvr
