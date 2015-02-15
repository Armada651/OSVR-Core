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

#ifndef INCLUDED_ServerPtr_h_GUID_ED06F093_FC19_49B1_8905_16E7CE12D207
#define INCLUDED_ServerPtr_h_GUID_ED06F093_FC19_49B1_8905_16E7CE12D207

// Internal Includes
#include <osvr/Util/SharedPtr.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace server {
    class Server;
    /// @brief How one should hold a Server.
    typedef shared_ptr<Server> ServerPtr;
    /// @brief How one might observe a Server without owning it.
    typedef weak_ptr<Server> ServerWeakPtr;
} // namespace server
} // namespace osvr

#endif // INCLUDED_ServerPtr_h_GUID_ED06F093_FC19_49B1_8905_16E7CE12D207
