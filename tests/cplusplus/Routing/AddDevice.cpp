/** @file
    @brief Test Implementation

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
#include "IsType.h"
#include <osvr/Routing/PathTree.h>
#include <osvr/Routing/AddDevice.h>
#include <osvr/Routing/Exceptions.h>

// Library/third-party includes
#include <boost/variant/get.hpp>
#include "gtest/gtest.h"

// Standard includes
// - none

using std::string;
using namespace osvr::routing;

TEST(addDevice, normalConditions) {
    PathTree tree;
    PathNode *dev = nullptr;
    ASSERT_NO_THROW(dev = &addDevice(tree, "/org_opengoggles_sample/MyDevice"));

    // Check Device
    ASSERT_EQ(dev->getName(), "MyDevice");
    ASSERT_FALSE(dev->hasChildren()) << "Make sure it has no children.";
    ASSERT_TRUE(isNodeType<elements::DeviceElement>(*dev)) << "Check type";
    ASSERT_NE(dev->getParent(), nullptr) << "Make sure it has a parent.";

    // Check org_opengoggles_sample
    auto plugin = dev->getParent();
    ASSERT_EQ(plugin->getName(), "org_opengoggles_sample");
    ASSERT_TRUE(isNodeType<elements::PluginElement>(*plugin)) << "Check type";
    ASSERT_NE(plugin->getParent(), nullptr) << "Make sure it has a parent.";
    auto root = plugin->getParent();

    ASSERT_TRUE(root->isRoot());
    ASSERT_EQ(tree.getNodeByPath("/"), *root)
        << "Root identity should be preserved";
    ASSERT_EQ(tree.getNodeByPath("/org_opengoggles_sample/MyDevice"), *dev)
        << "Identity should be preserved";
}

TEST(addDevice, missingLeadingSlash) {
    PathTree tree;

    PathNode *dev = nullptr;
    ASSERT_NO_THROW(dev = &addDevice(tree, "org_opengoggles_sample/MyDevice"))
        << "Should forgive a missing leading slash";
    ASSERT_EQ(tree.getNodeByPath("/org_opengoggles_sample/MyDevice"), *dev)
        << "Should be the same as if the slash had been present";
}

TEST(addDevice, BadInput) {
    PathTree tree;

    ASSERT_THROW(addDevice(tree, ""), exceptions::InvalidDeviceName)
        << "Should reject an empty path";
    ASSERT_THROW(addDevice(tree, "/"), exceptions::InvalidDeviceName)
        << "Should reject the root";
    ASSERT_THROW(addDevice(tree, "/org_opengoggles_sample"),
                 exceptions::InvalidDeviceName)
        << "Should reject just a single level";
    ASSERT_THROW(addDevice(tree, "/org_opengoggles_sample/"),
                 exceptions::InvalidDeviceName)
        << "Should reject just a single level with trailing slash";
    ASSERT_THROW(addDevice(tree, "org_opengoggles_sample"),
                 exceptions::InvalidDeviceName)
        << "Should reject just a single level w/o leading slash";
    ASSERT_THROW(addDevice(tree, "org_opengoggles_sample/"),
                 exceptions::InvalidDeviceName)
        << "Should reject just a single level with trailing but w/o leading "
           "slash";

    ASSERT_THROW(addDevice(tree, "/org_opengoggles_sample//"),
                 exceptions::EmptyPathComponent)
        << "Should reject empty second level";
    ASSERT_THROW(addDevice(tree, "org_opengoggles_sample//"),
                 exceptions::EmptyPathComponent)
        << "Should reject empty second level";
    ASSERT_THROW(addDevice(tree, "//"), exceptions::InvalidDeviceName)
        << "Too short to be legit.";
    ASSERT_THROW(addDevice(tree, "///"), exceptions::EmptyPathComponent);
}
