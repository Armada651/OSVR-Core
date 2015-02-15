/** @file
    @brief Header to bring unique_ptr into the ::osvr namespace.

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

#ifndef INCLUDED_UniquePtr_h_GUID_3CF3C525_BAAB_41B2_D659_A4D166EABB52
#define INCLUDED_UniquePtr_h_GUID_3CF3C525_BAAB_41B2_D659_A4D166EABB52

#if defined(_MSC_VER) && (_MSC_VER < 1600)
#error "Not supported before VS 2010"

#else
#include <memory>

namespace osvr {
using std::unique_ptr;
} // namespace osvr

#endif

#endif // INCLUDED_UniquePtr_h_GUID_3CF3C525_BAAB_41B2_D659_A4D166EABB52
