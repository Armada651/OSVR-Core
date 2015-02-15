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

#ifndef INCLUDED_SyncDeviceToken_h_GUID_0A738016_90A8_4E81_B5C0_247478D59FD2
#define INCLUDED_SyncDeviceToken_h_GUID_0A738016_90A8_4E81_B5C0_247478D59FD2

// Internal Includes
#include <osvr/Connection/DeviceToken.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace connection {
    class SyncDeviceToken : public OSVR_DeviceTokenObject {
      public:
        SyncDeviceToken(std::string const &name);
        virtual ~SyncDeviceToken();

      protected:
        virtual void m_setUpdateCallback(DeviceUpdateCallback const &cb);
        void m_sendData(util::time::TimeValue const &timestamp,
                        MessageType *type, const char *bytestream, size_t len);
        virtual GuardPtr m_getSendGuard();
        virtual void m_connectionInteract();

      private:
        DeviceUpdateCallback m_cb;
    };
} // namespace connection
} // namespace osvr
#endif // INCLUDED_SyncDeviceToken_h_GUID_0A738016_90A8_4E81_B5C0_247478D59FD2
