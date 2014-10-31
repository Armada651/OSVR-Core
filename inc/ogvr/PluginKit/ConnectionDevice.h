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

#ifndef INCLUDED_ConnectionDevice_h_GUID_C04223D7_51D1_49BE_91AB_73FCA9C88515
#define INCLUDED_ConnectionDevice_h_GUID_C04223D7_51D1_49BE_91AB_73FCA9C88515

// Internal Includes
#include <ogvr/PluginKit/ConnectionDevicePtr.h>

// Library/third-party includes
#include <boost/noncopyable.hpp>

// Standard includes
#include <string>

namespace ogvr {
/// @brief Base class for connection-specific device data, owned by a
/// DeviceToken.
class ConnectionDevice : boost::noncopyable {
  public:
    /// @brief destructor
    virtual ~ConnectionDevice();

    /// @brief accessor for device name
    std::string const &getName() const;

    /// @brief Process messages. This shouldn't block.
    ///
    /// Someone needs to call this method frequently.
    void process();

  protected:
    /// @brief (Subclass implementation) Process messages. This shouldn't block.
    virtual void m_process() = 0;

    /// @brief Constructor for use by derived classes only.
    ConnectionDevice(std::string const &name);

  private:
    std::string const m_name;
};
} // end of namespace ogvr
#endif // INCLUDED_ConnectionDevice_h_GUID_C04223D7_51D1_49BE_91AB_73FCA9C88515