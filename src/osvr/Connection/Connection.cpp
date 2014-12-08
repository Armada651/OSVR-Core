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
#include <osvr/Connection/Connection.h>
#include <osvr/Connection/ConnectionDevice.h>
#include <osvr/Util/SharedPtr.h>
#include <osvr/PluginHost/RegistrationContext.h>
#include <osvr/Connection/MessageType.h>
#include "VrpnBasedConnection.h"
#include "GenericConnectionDevice.h"

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
#include <boost/range/algorithm.hpp>
#include <boost/assert.hpp>

// Standard includes
// - none

namespace osvr {
namespace connection {
    /// @brief Internal constant string used as key into AnyMap
    static const char CONNECTION_KEY[] = "org.opengoggles.ConnectionPtr";

    ConnectionPtr Connection::createLocalConnection() {
        ConnectionPtr conn(make_shared<VrpnBasedConnection>(
            VrpnBasedConnection::VRPN_LOCAL_ONLY));
        return conn;
    }
    ConnectionPtr Connection::createSharedConnection() {
        ConnectionPtr conn(
            make_shared<VrpnBasedConnection>(VrpnBasedConnection::VRPN_SHARED));
        return conn;
    }

    ConnectionPtr
    Connection::retrieveConnection(const pluginhost::RegistrationContext &ctx) {
        ConnectionPtr ret;
        boost::any anyConn = ctx.data().get(CONNECTION_KEY);
        if (anyConn.empty()) {
            return ret;
        }
        ConnectionPtr *retPtr = boost::any_cast<ConnectionPtr>(&anyConn);
        if (retPtr) {
            ret = *retPtr;
        }
        return ret;
    }

    void Connection::storeConnection(pluginhost::RegistrationContext &ctx,
                                     ConnectionPtr conn) {
        ctx.data().set(CONNECTION_KEY, conn);
    }

    /// Wraps the derived implementation for future expandability.
    MessageTypePtr
    Connection::registerMessageType(std::string const &messageId) {
        return m_registerMessageType(messageId);
    }

    /// Wraps the derived implementation for future expandability.
    ConnectionDevicePtr
    Connection::registerDevice(std::string const &deviceName) {
        ConnectionDevicePtr dev = m_registerDevice(deviceName);
        if (dev) {
            addDevice(dev);
        }
        return dev;
    }
    ConnectionDevicePtr Connection::registerAdvancedDevice(
        std::string const &deviceName,
        OSVR_SyncDeviceUpdateCallback updateFunction, void *userdata) {
        ConnectionDevicePtr dev(new GenericConnectionDevice(
            deviceName, std::bind(updateFunction, userdata)));
        addDevice(dev);
        return dev;
    }

    void Connection::addDevice(ConnectionDevicePtr device) {
        BOOST_ASSERT_MSG(device, "Device must be non-null!");
        m_devices.push_back(device);
    }

    void Connection::process() {
        // Process the connection first.
        m_process();
        // Process all devices.
        for (auto &dev : m_devices) {
            dev->process();
        }
    }

    Connection::Connection() { OSVR_DEV_VERBOSE("In Connection constructor"); }

    Connection::~Connection() { OSVR_DEV_VERBOSE("In Connection destructor"); }

    void *Connection::getUnderlyingObject() { return nullptr; }

    const char *Connection::getConnectionKindID() { return nullptr; }

} // namespace connection
} // namespace osvr
