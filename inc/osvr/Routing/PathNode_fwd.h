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

#ifndef INCLUDED_PathNode_fwd_h_GUID_3160EFE4_EC7A_4187_5263_ED3303384F7B
#define INCLUDED_PathNode_fwd_h_GUID_3160EFE4_EC7A_4187_5263_ED3303384F7B

// Internal Includes
#if !defined(__ANDROID__)
#include <osvr/Routing/Export.h>
#else
#define OSVR_ROUTING_EXPORT __attribute__((visibility("default")))
#endif
#include <osvr/Routing/PathElementTypes_fwd.h>
#include <osvr/Util/TreeNode_fwd.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace routing {
    /// @brief The specific tree node type that contains a path element.
    typedef ::osvr::util::TreeNode<PathElement> PathNode;

    /// @brief The ownership pointer of the specific tree node.
    typedef ::osvr::util::TreeNodePointer<PathElement>::type PathNodePtr;

    /// @brief The weak (non-owning) pointer used to refer to a parent tree node
    typedef PathNode *PathNodeParentPtr;

    /// @brief Gets an identifying string for the node value type.
    OSVR_ROUTING_EXPORT const char *getTypeName(PathNode const &node);
} // namespace routing
} // namespace osvr

#endif // INCLUDED_PathNode_fwd_h_GUID_3160EFE4_EC7A_4187_5263_ED3303384F7B
