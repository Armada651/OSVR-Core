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

#ifndef INCLUDED_GetVRPNConnection_h_GUID_008AFE04_5378_43B9_BE78_571F7C16D2DD
#define INCLUDED_GetVRPNConnection_h_GUID_008AFE04_5378_43B9_BE78_571F7C16D2DD

// Internal Includes
#include <osvr/VRPNServer/Export.h>
#include <osvr/PluginHost/PluginSpecificRegistrationContext.h>
#include <osvr/Util/PluginRegContextC.h>

// Library/third-party includes
// - none

// Standard includes
// - none

class vrpn_Connection;

namespace osvr {

/// @addtogroup VRPNServer
/// @{
/// @brief Contains functions and classes for @ref VRPNServer
namespace vrpnserver {
    /// @brief Retrieves the vrpn_Connection pointer from an
    /// OSVR_PluginRegContext.
    /// @returns the pointer, or null if this isn't a VRPN connection or
    /// something else went wrong.
    OSVR_VRPNSERVER_EXPORT vrpn_Connection *
    getVRPNConnection(OSVR_PluginRegContext ctx);

    /// @overload
    OSVR_VRPNSERVER_EXPORT vrpn_Connection *getVRPNConnection(
        osvr::pluginhost::PluginSpecificRegistrationContext &context);
} // namespace vrpnserver
/// @}
} // namespace osvr

#endif // INCLUDED_GetVRPNConnection_h_GUID_008AFE04_5378_43B9_BE78_571F7C16D2DD
