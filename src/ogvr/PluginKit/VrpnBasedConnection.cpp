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
#include <ogvr/PluginKit/ConnectionDevice.h>
#include <ogvr/Util/UniquePtr.h>
#include <ogvr/Util/Verbosity.h>

// Library/third-party includes
#include <vrpn_BaseClass.h>
#include <boost/range/algorithm.hpp>

// Standard includes
// - none

namespace ogvr {

namespace {
    class vrpn_BaseFlexServer : public vrpn_BaseClass {
      public:
        vrpn_BaseFlexServer(const char *name, vrpn_Connection *conn)
            : vrpn_BaseClass(name, conn) {
            vrpn_BaseClass::init();
        }
        virtual ~vrpn_BaseFlexServer() {}

        virtual void mainloop() {
            OGVR_DEV_VERBOSE("In vrpn_BaseFlexServer::mainloop");
            /// @todo service device here
            server_mainloop();
        }
        void sendData(vrpn_uint32 msgID, const char *bytestream, size_t len) {
            struct timeval now;
            vrpn_gettimeofday(&now, NULL);
            d_connection->pack_message(len, now, msgID, d_sender_id, bytestream,
                                       vrpn_CONNECTION_LOW_LATENCY);
        }

      protected:
        virtual int register_types() { return 0; }
    };

    class VrpnConnectionDevice : public ConnectionDevice {
      public:
        VrpnConnectionDevice(std::string const &name,
                             vrpn_ConnectionPtr const &vrpnConn)
            : ConnectionDevice(name) {
            m_baseobj.reset(
                new vrpn_BaseFlexServer(name.c_str(), vrpnConn.get()));
        }
        virtual ~VrpnConnectionDevice() {}
        virtual void m_process() {
            OGVR_DEV_VERBOSE("In VrpnConnectionDevice::m_process");
            m_baseobj->mainloop();
        }
        virtual void m_sendData(MessageType *type, const char *bytestream,
                                size_t len) {
            VrpnMessageType *msgtype = static_cast<VrpnMessageType *>(type);
            m_baseobj->sendData(msgtype->getID(), bytestream, len);
        }

      private:
        unique_ptr<vrpn_BaseFlexServer> m_baseobj;
    };
}

VrpnBasedConnection::VrpnBasedConnection(ConnectionType type) {
    switch (type) {
    case VRPN_LOCAL_ONLY: {
        m_vrpnConnection =
            vrpn_ConnectionPtr::create_server_connection("127.0.0.1");
    }
    case VRPN_SHARED: {
        m_vrpnConnection = vrpn_ConnectionPtr::create_server_connection();
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
    m_devices.push_back(ret);
    return ret;
}

void VrpnBasedConnection::m_process() {
    m_vrpnConnection->mainloop();
    boost::for_each(m_devices,
                    [](ConnectionDevicePtr &dev) { dev->process(); });
}

VrpnBasedConnection::~VrpnBasedConnection() {
    /// @todo wait until all async threads are done
}
}