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

#ifndef INCLUDED_WrapRoute_h_GUID_CC93B3E1_2C30_42AE_9880_FE43AD40AB81
#define INCLUDED_WrapRoute_h_GUID_CC93B3E1_2C30_42AE_9880_FE43AD40AB81

// Internal Includes
#include <osvr/Routing/RoutingKeys.h>

// Library/third-party includes
#include <json/value.h>

// Standard includes
// - none

inline Json::Value wrapRoute(Json::Value currentRoute,
                             Json::Value const &newLevel) {
    Json::Value ret = currentRoute;
    Json::Value prevSource = currentRoute[osvr::routing::keys::source()];
    ret[osvr::routing::keys::source()] = newLevel;
    ret[osvr::routing::keys::source()]["child"] = prevSource;
    return ret;
}

#endif // INCLUDED_WrapRoute_h_GUID_CC93B3E1_2C30_42AE_9880_FE43AD40AB81
