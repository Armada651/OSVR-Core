/** @file
    @brief Header

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

#ifndef INCLUDED_ConnectionDevice_h_GUID_C04223D7_51D1_49BE_91AB_73FCA9C88515
#define INCLUDED_ConnectionDevice_h_GUID_C04223D7_51D1_49BE_91AB_73FCA9C88515

// Internal Includes
#if !defined(__ANDROID__)
#include <osvr/Connection/Export.h>
#else
#define OSVR_CONNECTION_EXPORT __attribute__((visibility("default")))
#endif
#include <osvr/Connection/ConnectionDevicePtr.h>
#include <osvr/Util/TimeValue.h>

// Library/third-party includes
#include <boost/noncopyable.hpp>

// Standard includes
#include <string>

namespace osvr {
namespace connection {
    class MessageType;
    class DeviceToken;

    /// @brief Base class for connection-specific device data, owned by a
    /// DeviceToken.
    class ConnectionDevice : boost::noncopyable {
      public:
        /// @brief destructor
        OSVR_CONNECTION_EXPORT virtual ~ConnectionDevice();

        /// @brief accessor for device name
        std::string const &getName() const;

        /// @brief Process messages. This shouldn't block.
        ///
        /// Someone needs to call this method frequently.
        void process();

        /// @brief Send message.
        void sendData(util::time::TimeValue const &timestamp, MessageType *type,
                      const char *bytestream, size_t len);

        /// @brief For use only by DeviceToken
        void setDeviceToken(DeviceToken &token);

      protected:
        /// @brief accessor for device token
        DeviceToken &m_getDeviceToken();
        /// @brief (Subclass implementation) Process messages. This shouldn't
        /// block.
        virtual void m_process() = 0;

        /// @brief (Subclass implementation) Send message.
        virtual void m_sendData(util::time::TimeValue const &timestamp,
                                MessageType *type, const char *bytestream,
                                size_t len) = 0;

        /// @brief Constructor for use by derived classes only.
        OSVR_CONNECTION_EXPORT ConnectionDevice(std::string const &name);

      private:
        std::string const m_name;
        DeviceToken *m_token;
    };
} // namespace connection
} // namespace osvr
#endif // INCLUDED_ConnectionDevice_h_GUID_C04223D7_51D1_49BE_91AB_73FCA9C88515
