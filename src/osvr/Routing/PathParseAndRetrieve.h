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

#ifndef INCLUDED_PathParseAndRetrieve_h_GUID_C451663C_0711_4B85_2011_61D26E5C237C
#define INCLUDED_PathParseAndRetrieve_h_GUID_C451663C_0711_4B85_2011_61D26E5C237C

// Internal Includes
#include <osvr/Routing/PathNode_fwd.h>

// Library/third-party includes
// - none

// Standard includes
#include <string>

namespace osvr {
namespace routing {
    /// @brief Internal method for parsing a path and getting or creating the
    /// nodes along it.
    /// @param path An absolute path (beginning with /) - a trailing slash is
    /// trimmed silently
    /// @param root The root node of a tree. This is not checked at runtime
    /// (just a debug assert) since this should only be called from safe,
    /// internal locations!
    /// @returns a reference to the leaf node referred to by the path.
    /// @throws exceptions::PathNotAbsolute, exceptions::EmptyPath,
    /// exceptions::EmptyPathComponent
    PathNode &pathParseAndRetrieve(std::string const &path, PathNode &root);
} // namespace routing
} // namespace osvr

#endif // INCLUDED_PathParseAndRetrieve_h_GUID_C451663C_0711_4B85_2011_61D26E5C237C
