/** @file
    @brief Header

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

#ifndef INCLUDED_Connection_h_GUID_61C65986_E2C9_498F_59F0_8EFC712BA183
#define INCLUDED_Connection_h_GUID_61C65986_E2C9_498F_59F0_8EFC712BA183

// Internal Includes
#include <osvr/Connection/Export.h>
#include <osvr/Connection/MessageTypePtr.h>
#include <osvr/Connection/ConnectionDevicePtr.h>
#include <osvr/Connection/ConnectionPtr.h>
#include <osvr/Connection/DeviceInitObject.h>
#include <osvr/Util/DeviceCallbackTypesC.h>
#include <osvr/PluginHost/RegistrationContext_fwd.h>

// Library/third-party includes
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

// Standard includes
#include <string>
#include <vector>
#include <functional>

namespace osvr {
/// @brief Messaging transport and device communication functionality
/// @ingroup Connection
namespace connection {

    /// @brief Class wrapping a messaging transport (server or internal)
    /// connection.
    class Connection : boost::noncopyable,
                       public enable_shared_from_this<Connection> {
      public:
        /// @name Factory methods
        ///
        /// More to come.
        /// @{
        /// @brief Factory method to create a local-machine-only connection
        OSVR_CONNECTION_EXPORT static ConnectionPtr createLocalConnection();
        /// @brief Factory method to create a shared connection
        /// @param iface The NIC/interface to listen on, an empty string
        /// or unset (default) means "all interfaces"
        /// @param port The port to listen on, unset/default means the default
        /// port for the underlying connection implementation.
        OSVR_CONNECTION_EXPORT static ConnectionPtr
        createSharedConnection(boost::optional<std::string const &> iface,
                               boost::optional<int> port);
        /// @}

        /// @name Context Storage
        /// @{
        /// @brief Retrieve a connection pointer from a RegistrationContext
        OSVR_CONNECTION_EXPORT static ConnectionPtr
        retrieveConnection(const pluginhost::RegistrationContext &ctx);
        /// @brief Store a connection pointer in a RegistrationContext
        OSVR_CONNECTION_EXPORT static void
        storeConnection(pluginhost::RegistrationContext &ctx,
                        ConnectionPtr conn);
        /// @}

        /// @brief Register (or retrieve registration) of a message type.
        OSVR_CONNECTION_EXPORT MessageTypePtr
        registerMessageType(std::string const &messageId);

        /// @brief Create a ConnectionDevice by registering a full device name.
        /// This should be namespaced with the plugin name.
        ///
        /// This also adds the device so created to the device list.
        ///
        /// ConnectionDevices often assume they're owned by a DeviceToken (whose
        /// constructor calls this method), so doing otherwise is unadvisable.
        OSVR_CONNECTION_EXPORT ConnectionDevicePtr
        createConnectionDevice(std::string const &deviceName);

        /// @overload
        OSVR_CONNECTION_EXPORT ConnectionDevicePtr
        createConnectionDevice(DeviceInitObject &init);

        /// @brief Add an externally-constructed device to the device list.
        OSVR_CONNECTION_EXPORT void addDevice(ConnectionDevicePtr device);

        /// @brief Process messages. This shouldn't block.
        ///
        /// Someone needs to call this method frequently.
        OSVR_CONNECTION_EXPORT void process();

        /// @brief Register a function to be called when a client connects or
        /// pings.
        OSVR_CONNECTION_EXPORT void
        registerConnectionHandler(std::function<void()> handler);

        /// @brief Destructor
        OSVR_CONNECTION_EXPORT virtual ~Connection();

        /// @name Advanced Methods - not for general consumption
        /// These can break encapsulation rules and/or encourage bad coding
        /// habits.
        /// Avoid if at all possible.
        /// @{

        /// @brief Record a full device name (namespaced with the plugin name)
        /// associated with a given callback.
        ///
        /// This does _not_ register the name in the underlying connection: it
        /// is an advanced method that assumes you have some other way of doing
        /// that.
        ///
        /// This also adds the device so created to the device list.
        OSVR_CONNECTION_EXPORT ConnectionDevicePtr
        registerAdvancedDevice(std::string const &deviceName,
                               OSVR_DeviceUpdateCallback updateFunction,
                               void *userdata);

        /// @brief Type of list of device names.
        typedef std::vector<std::string> NameList;

        /// @brief Record more than one full device name (namespaced with the
        /// plugin name) associated with a given callback.
        ///
        /// This does _not_ register the names in the underlying connection: it
        /// is an advanced method that assumes you have some other way of doing
        /// that.
        ///
        /// This also adds the device so created to the device list.
        ///
        /// For use when a single device exposes more than one name.
        OSVR_CONNECTION_EXPORT ConnectionDevicePtr
        registerAdvancedDevice(NameList const &deviceNames,
                               OSVR_DeviceUpdateCallback updateFunction,
                               void *userdata);

        /// @brief Access implementation details.
        OSVR_CONNECTION_EXPORT virtual void *getUnderlyingObject();

        /// @brief Returns some implementation-defined string based on the
        /// dynamic type of the connection.
        OSVR_CONNECTION_EXPORT virtual const char *getConnectionKindID();
        /// @}
      protected:
        /// @brief (Subclass implementation) Register (or retrieve registration)
        /// of a message type.
        virtual MessageTypePtr
        m_registerMessageType(std::string const &messageId) = 0;

        /// @brief (Subclass implementation) Register a full device name.
        virtual ConnectionDevicePtr
        m_createConnectionDevice(DeviceInitObject &init) = 0;

        /// @brief (Subclass implementation) Register a function to handle "new
        /// connection"/ping messages.
        virtual void
        m_registerConnectionHandler(std::function<void()> handler) = 0;

        /// @brief (Subclass implementation) Process messages. This shouldn't
        /// block.
        virtual void m_process() = 0;

        /// brief Constructor
        Connection();

      private:
        typedef std::vector<ConnectionDevicePtr> DeviceList;
        DeviceList m_devices;
    };
} // namespace connection
} // namespace osvr
#endif // INCLUDED_Connection_h_GUID_61C65986_E2C9_498F_59F0_8EFC712BA183
