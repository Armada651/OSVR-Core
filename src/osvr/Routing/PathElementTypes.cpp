/** @file
    @brief Implementation

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

// Internal Includes
#include <osvr/Routing/PathElementTypes.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace routing {
    namespace elements {
        void AliasElement::setTarget(std::string const &targetPath) {
            /// @todo validation?
            m_target = targetPath;
        }

        std::string const &AliasElement::getTarget() const { return m_target; }
    } // namespace elements
} // namespace routing
} // namespace osvr