/** @file
    @brief Header

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2015 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

#ifndef INCLUDED_ButtonServerInterface_h_GUID_864E9363_E9EE_42AD_E0D6_990E6E61B137
#define INCLUDED_ButtonServerInterface_h_GUID_864E9363_E9EE_42AD_E0D6_990E6E61B137

// Internal Includes
#include <osvr/Util/ChannelCountC.h>
#include <osvr/Util/TimeValue.h>
#include <osvr/Util/ClientReportTypesC.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace connection {
    class ButtonServerInterface {
      public:
        typedef OSVR_ButtonState value_type;
        /// @brief Sets the value to val at channel chan, and reports changes
        /// with the given timestamp
        virtual bool setValue(value_type val, OSVR_ChannelCount chan,
                              util::time::TimeValue const &timestamp) = 0;

        /// @brief Sets the values from the val array at channels [0, chans),
        /// and reports changes with the given timestamp
        virtual void setValues(value_type val[], OSVR_ChannelCount chans,
                               util::time::TimeValue const &timestamp) = 0;
    };

} // namespace connection
} // namespace osvr

#endif // INCLUDED_ButtonServerInterface_h_GUID_864E9363_E9EE_42AD_E0D6_990E6E61B137
