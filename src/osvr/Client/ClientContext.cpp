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

// Internal Includes
#include <osvr/Client/ClientContext.h>
#include <osvr/Client/ClientInterface.h>

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

// Library/third-party includes
#include <boost/assert.hpp>

// Standard includes
#include <algorithm>

using ::osvr::client::ClientInterfacePtr;
using ::osvr::client::ClientInterface;
using ::osvr::make_shared;

OSVR_ClientContextObject::OSVR_ClientContextObject(const char appId[])
    : m_appId(appId) {
    OSVR_DEV_VERBOSE("Client context initialized for " << m_appId);
}

OSVR_ClientContextObject::~OSVR_ClientContextObject() {}

std::string const &OSVR_ClientContextObject::getAppId() const {
    return m_appId;
}

void OSVR_ClientContextObject::update() {
    m_update();
    std::for_each(begin(m_interfaces), end(m_interfaces),
                  [](ClientInterfacePtr const &iface) { iface->update(); });
}

ClientInterfacePtr OSVR_ClientContextObject::getInterface(const char path[]) {
    ClientInterfacePtr ret;
    if (!path) {
        return ret;
    }
    std::string p(path);
    if (p.empty()) {
        return ret;
    }
    ret = make_shared<ClientInterface>(this, path,
                                       ClientInterface::PrivateConstructor());
    m_interfaces.push_back(ret);
    return ret;
}

ClientInterfacePtr
OSVR_ClientContextObject::releaseInterface(ClientInterface *iface) {
    ClientInterfacePtr ret;
    if (!iface) {
        return ret;
    }
    InterfaceList::iterator it =
        std::find_if(begin(m_interfaces), end(m_interfaces),
                     [&](ClientInterfacePtr const &ptr) {
            if (ptr.get() == iface) {
                ret = ptr;
                return true;
            }
            return false;
        });
    BOOST_ASSERT_MSG(
        (it == end(m_interfaces)) == (!ret),
        "We should have a pointer if and only if we have the iterator");
    if (ret) {
        // Erase it from our list
        m_interfaces.erase(it);
    }
    return ret;
}

std::string
OSVR_ClientContextObject::getStringParameter(std::string const &path) const {
    auto it = m_params.find(path);
    std::string ret;
    if (it != m_params.end()) {
        ret = it->second;
    }
    return ret;
}

void OSVR_ClientContextObject::setParameter(std::string const &path,
                                            std::string const &value) {
    OSVR_DEV_VERBOSE("Parameter set for " << path);
    m_params[path] = value;
}
