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

#ifndef INCLUDED_Constants_h_GUID_1A7E9AA8_23B3_409B_4589_A15870E9473D
#define INCLUDED_Constants_h_GUID_1A7E9AA8_23B3_409B_4589_A15870E9473D

// Internal Includes
#if !defined(__ANDROID__)
#include <osvr/Routing/Export.h>
#else
#define OSVR_ROUTING_EXPORT __attribute__((visibility("default")))
#endif

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace routing {
    /// @name Constants
    /// @{
    /// @brief Gets the path separator character - a slash.
    OSVR_ROUTING_EXPORT char getPathSeparatorCharacter();

    /// @brief Gets the path separator - a slash - as a null-terminated
    /// string.
    OSVR_ROUTING_EXPORT const char *getPathSeparator();
    /// @}
} // namespace routing
} // namespace osvr
#endif // INCLUDED_Constants_h_GUID_1A7E9AA8_23B3_409B_4589_A15870E9473D
