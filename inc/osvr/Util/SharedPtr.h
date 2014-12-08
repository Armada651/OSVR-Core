/** @file
    @brief Header to bring shared_ptr into the ::osvr namespace.

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

#ifndef INCLUDED_SharedPtr_h_GUID_E9C5BC8D_7D3A_4896_1552_6F4F5292783C
#define INCLUDED_SharedPtr_h_GUID_E9C5BC8D_7D3A_4896_1552_6F4F5292783C

#if defined(_MSC_VER) && (_MSC_VER < 1600)
#error "Not supported before VS 2010"
#else
#include <memory>

namespace osvr {
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::enable_shared_from_this;
} // namespace osvr
#endif

#endif // INCLUDED_SharedPtr_h_GUID_E9C5BC8D_7D3A_4896_1552_6F4F5292783C
