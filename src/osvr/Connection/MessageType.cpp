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
#include <osvr/Connection/MessageType.h>

// Library/third-party includes
// - none

// Standard includes
// - none

OSVR_MessageTypeObject::~OSVR_MessageTypeObject() {}

std::string const &OSVR_MessageTypeObject::getName() const { return m_name; }

OSVR_MessageTypeObject::OSVR_MessageTypeObject(std::string const &name)
    : m_name(name) {}