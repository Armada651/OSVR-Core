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
#include <osvr/Connection/DeviceToken.h>
#include "AsyncDeviceToken.h"
#include "SyncDeviceToken.h"
#include "VirtualDeviceToken.h"
#include <osvr/Connection/Connection.h>
#include <osvr/Connection/ConnectionDevice.h>

// Library/third-party includes
// - none

// Standard includes
#include <stdexcept>

using osvr::connection::DeviceTokenPtr;
using osvr::connection::DeviceInitObject;
using osvr::connection::AsyncDeviceToken;
using osvr::connection::SyncDeviceToken;
using osvr::connection::VirtualDeviceToken;
using osvr::connection::ConnectionPtr;
using osvr::connection::MessageType;
using osvr::connection::GuardPtr;
using osvr::connection::ConnectionDevicePtr;
DeviceTokenPtr
OSVR_DeviceTokenObject::createAsyncDevice(DeviceInitObject &init) {
    DeviceTokenPtr ret(new AsyncDeviceToken(init.getQualifiedName()));
    ret->m_sharedInit(init);
    return ret;
}

DeviceTokenPtr
OSVR_DeviceTokenObject::createSyncDevice(DeviceInitObject &init) {
    DeviceTokenPtr ret(new SyncDeviceToken(init.getQualifiedName()));
    ret->m_sharedInit(init);
    return ret;
}

DeviceTokenPtr
OSVR_DeviceTokenObject::createVirtualDevice(std::string const &name,
                                            ConnectionPtr const &conn) {
    DeviceInitObject init(conn);
    init.setName(name);
    DeviceTokenPtr ret(new VirtualDeviceToken(name));
    ret->m_sharedInit(init);
    return ret;
}

OSVR_DeviceTokenObject::OSVR_DeviceTokenObject(std::string const &name)
    : m_name(name) {}

OSVR_DeviceTokenObject::~OSVR_DeviceTokenObject() {}

std::string const &OSVR_DeviceTokenObject::getName() const { return m_name; }

void OSVR_DeviceTokenObject::sendData(MessageType *type, const char *bytestream,
                                      size_t len) {
    osvr::util::time::TimeValue tv;
    osvr::util::time::getNow(tv);
    m_sendData(tv, type, bytestream, len);
}
void OSVR_DeviceTokenObject::sendData(
    osvr::util::time::TimeValue const &timestamp, MessageType *type,
    const char *bytestream, size_t len) {
    m_sendData(timestamp, type, bytestream, len);
}

GuardPtr OSVR_DeviceTokenObject::getSendGuard() { return m_getSendGuard(); }

void OSVR_DeviceTokenObject::setUpdateCallback(
    osvr::connection::DeviceUpdateCallback const &cb) {
    m_setUpdateCallback(cb);
}

void OSVR_DeviceTokenObject::connectionInteract() { m_connectionInteract(); }

void OSVR_DeviceTokenObject::stopThreads() { m_stopThreads(); }

ConnectionPtr OSVR_DeviceTokenObject::m_getConnection() { return m_conn; }

ConnectionDevicePtr OSVR_DeviceTokenObject::m_getConnectionDevice() {
    return m_dev;
}

void OSVR_DeviceTokenObject::m_stopThreads() {}

void OSVR_DeviceTokenObject::m_sharedInit(DeviceInitObject &init) {
    m_conn = init.getConnection();
    m_dev = m_conn->createConnectionDevice(init);
    m_dev->setDeviceToken(*this);
}
