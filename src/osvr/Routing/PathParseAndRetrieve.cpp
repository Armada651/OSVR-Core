/** @file
    @brief Implementation

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

#define OSVR_DEV_VERBOSE_DISABLE

// Internal Includes
#include "PathParseAndRetrieve.h"
#include <osvr/Routing/Constants.h>
#include <osvr/Routing/PathNode.h>
#if !defined(__ANDROID__)
#include <osvr/Util/Verbosity.h>
#else
#include <osvr/Util/MacroToolsC.h>
#if defined(OSVR_UTIL_DEV_VERBOSE) && !defined(OSVR_DEV_VERBOSE_DISABLE)
#include <iostream>
#define OSVR_DEV_VERBOSE(X)                                                    \
    OSVR_UTIL_MULTILINE_BEGIN std::cout << "[OSVR] " << X << std::endl;        \
    OSVR_UTIL_MULTILINE_END
#else
#define OSVR_DEV_VERBOSE(X) OSVR_UTIL_MULTILINE_BEGIN OSVR_UTIL_MULTILINE_END
#endif
#endif
#include <osvr/Routing/Exceptions.h>

// Library/third-party includes
#include <boost/assert.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/range/adaptor/sliced.hpp>

// Standard includes
// - none

namespace osvr {
namespace routing {
    PathNode &pathParseAndRetrieve(std::string const &path, PathNode &root) {

        using std::string;
        using boost::split_iterator;
        using boost::algorithm::make_split_iterator;
        using boost::adaptors::sliced;
        using boost::first_finder;
        using boost::is_equal;
        BOOST_ASSERT_MSG(root.isRoot(), "Must pass the root node!");
        if (path.empty()) {
            throw exceptions::EmptyPath();
        }
        if (path == getPathSeparator()) {
            /// @todo is an empty path valid input?
            return root;
        }
        if (path.at(0) != getPathSeparatorCharacter()) {
            throw exceptions::PathNotAbsolute(path);
        }
        PathNode *ret = &root;

        // Determine if there's a trailing slash.
        const size_t len = path.size();
        const size_t theEnd =
            (path[len - 1] == getPathSeparatorCharacter()) ? len - 1 : len;

        // Get the boost range that excludes the leading slash, and a trailing
        // slash, if any.
        auto range_excluding_leading_slash = path | sliced(1, theEnd);
        OSVR_DEV_VERBOSE(
            "Range excluding leading slash: "
            << boost::copy_range<std::string>(range_excluding_leading_slash));
        // Iterate through the chunks of the path, split by a slash.
        std::string component;
        typedef split_iterator<string::const_iterator> string_split_iterator;
        for (string_split_iterator It = make_split_iterator(
                 range_excluding_leading_slash,
                 first_finder(getPathSeparator(), is_equal()));
             It != string_split_iterator(); ++It) {
            component = boost::copy_range<std::string>(*It);
            OSVR_DEV_VERBOSE(component);
            if (component.empty()) {
                throw exceptions::EmptyPathComponent(path);
            }
            ret = &(ret->getOrCreateChildByName(component));
        }
        return *ret;
    }
} // namespace routing
} // namespace osvr
