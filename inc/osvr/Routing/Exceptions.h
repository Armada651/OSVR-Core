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

#ifndef INCLUDED_Exceptions_h_GUID_DDBD7E30_8C15_46AE_4221_1EB366EA079D
#define INCLUDED_Exceptions_h_GUID_DDBD7E30_8C15_46AE_4221_1EB366EA079D

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <stdexcept>

namespace osvr {
namespace routing {
    /// @brief Exceptions that may be thrown from this module.
    ///
    /// @ingroup Routing
    namespace exceptions {
        /// @brief Thrown when attempting to add an invalid device name to the
        /// tree.
        struct InvalidDeviceName : std::runtime_error {
            InvalidDeviceName(std::string const &details)
                : std::runtime_error(
                      "Attempted to add invalid device name to tree: " +
                      details) {}
        };

        /// @brief Thrown when attempting to use a path with an empty component
        struct EmptyPathComponent : std::runtime_error {
            EmptyPathComponent(std::string const &path)
                : std::runtime_error(
                      "Cannot use a path with an empty component: " + path) {}
        };

        /// @brief Thrown when attempting to use an empty path
        struct EmptyPath : std::runtime_error {
            EmptyPath()
                : std::runtime_error("Cannot retrieve an empty path!") {}
        };

        /// @brief Thrown when attempting to use a path with no leading slash
        /// where an absolute path is required.
        struct PathNotAbsolute : std::runtime_error {
            PathNotAbsolute(std::string const &path)
                : std::runtime_error(
                      "Provided path was not absolute (no leading slash): " +
                      path) {}
        };
    } // namespace exceptions
} // namespace routing
} // namespace osvr
#endif // INCLUDED_Exceptions_h_GUID_DDBD7E30_8C15_46AE_4221_1EB366EA079D
