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
#include "VrpnBasedConnection.h"
#include "VrpnMessageType.h"
#include "VrpnConnectionDevice.h"

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

    VrpnBasedConnection::VrpnBasedConnection(ConnectionType type) {
        switch (type) {
        case VRPN_LOCAL_ONLY: {
            m_vrpnConnection =
                vrpn_ConnectionPtr::create_server_connection("localhost");
            break;
        }
        case VRPN_SHARED: {
            m_vrpnConnection = vrpn_ConnectionPtr::create_server_connection();
            break;
        }
        }
    }

    MessageTypePtr
    VrpnBasedConnection::m_registerMessageType(std::string const &messageId) {
        MessageTypePtr ret(new VrpnMessageType(messageId, m_vrpnConnection));
        return ret;
    }

    ConnectionDevicePtr
    VrpnBasedConnection::m_registerDevice(std::string const &deviceName) {
        ConnectionDevicePtr ret =
            make_shared<VrpnConnectionDevice>(deviceName, m_vrpnConnection);
        return ret;
    }

    void VrpnBasedConnection::m_process() { m_vrpnConnection->mainloop(); }

    VrpnBasedConnection::~VrpnBasedConnection() {
        /// @todo wait until all async threads are done
    }

    void *VrpnBasedConnection::getUnderlyingObject() {
        return static_cast<void *>(m_vrpnConnection.get());
    }

    OSVR_CONNECTION_EXPORT const char *getVRPNConnectionKindID();

    const char *getVRPNConnectionKindID() { return "org.opengoggles.vrpn"; }

    const char *VrpnBasedConnection::getConnectionKindID() {
        return getVRPNConnectionKindID();
    }

} // namespace connection
} // namespace osvr
