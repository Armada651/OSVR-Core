/** @file
    @brief Implementation of the "multiserver" plugin that offers the stock VRPN
   devices.

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

// Define this for verbose output during polling.
#undef OSVR_MULTISERVER_VERBOSE

// Internal Includes
#include "VRPNMultiserver.h"
#include "DevicesWithParameters.h"
#include <osvr/PluginKit/PluginKit.h>
#include <osvr/Util/UniquePtr.h>
#include <osvr/VRPNServer/VRPNDeviceRegistration.h>

// Library/third-party includes
#include "hidapi/hidapi.h"
#include "vrpn_Connection.h"
#include "vrpn_Tracker_RazerHydra.h"
#include "vrpn_Tracker_OSVRHackerDevKit.h"
#include "vrpn_Tracker_Filter.h"
#include <boost/noncopyable.hpp>

#ifdef OSVR_MULTISERVER_VERBOSE
#include <boost/format.hpp>
#endif

// Standard includes
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#ifdef OSVR_MULTISERVER_VERBOSE
#include <iostream>
#endif

class VRPNHardwareDetect : boost::noncopyable {
  public:
    VRPNHardwareDetect(VRPNMultiserverData &data) : m_data(data) {}
    OSVR_ReturnCode operator()(OSVR_PluginRegContext ctx) {
        bool gotDevice;
#ifdef OSVR_MULTISERVER_VERBOSE
        bool first = true;
#endif
        do {
            gotDevice = false;
            struct hid_device_info *enumData = hid_enumerate(0, 0);
            for (struct hid_device_info *dev = enumData; dev != nullptr;
                 dev = dev->next) {

                if (m_isPathHandled(dev->path)) {
                    continue;
                }

#ifdef OSVR_MULTISERVER_VERBOSE
                if (first) {
                    std::cout << "[OSVR Multiserver] HID Enumeration: "
                              << boost::format("0x%04x") % dev->vendor_id << ":"
                              << boost::format("0x%04x") % dev->product_id
                              << std::endl;
                }
#endif

                if (gotDevice) {
                    continue;
                }
                // Razer Hydra
                if (dev->vendor_id == 0x1532 && dev->product_id == 0x0300) {
                    gotDevice = true;
                    m_handlePath(dev->path);
                    /// Decorated name for Hydra
                    std::string name;
                    {
                        // Razer Hydra
                        osvr::vrpnserver::VRPNDeviceRegistration reg(ctx);
                        name =
                            reg.useDecoratedName(m_data.getName("RazerHydra"));
                        reg.registerDevice(new vrpn_Tracker_RazerHydra(
                            name.c_str(), reg.getVRPNConnection()));
                    }
                    std::string localName = "*" + name;

                    {
                        // Corresponding filter
                        osvr::vrpnserver::VRPNDeviceRegistration reg(ctx);
                        reg.registerDevice(new vrpn_Tracker_FilterOneEuro(
                            reg.useDecoratedName(
                                    m_data.getName("OneEuroFilter")).c_str(),
                            reg.getVRPNConnection(), localName.c_str(), 2, 1.15,
                            1.0, 1.2, 1.5, 5.0, 1.2));
                    }
                    continue;
                }

                // OSVR Hacker Dev Kit
                if ((dev->vendor_id == 0x1532 && dev->product_id == 0x0300) ||
                    (dev->vendor_id == 0x03EB && dev->product_id == 0x2421)) {
                    gotDevice = true;
                    m_handlePath(dev->path);
                    osvr::vrpnserver::VRPNDeviceRegistration reg(ctx);
                    reg.constructAndRegisterDevice<
                        vrpn_Tracker_OSVRHackerDevKit>(
                        m_data.getName("OSVRHackerDevKit"));
                    continue;
                }
            }
            hid_free_enumeration(enumData);

#ifdef OSVR_MULTISERVER_VERBOSE
            first = false;
#endif

        } while (gotDevice);
        return OSVR_RETURN_SUCCESS;
    }

  private:
    bool m_isPathHandled(const char *path) {
        return std::find(begin(m_handledPaths), end(m_handledPaths),
                         std::string(path)) != end(m_handledPaths);
    }
    void m_handlePath(const char *path) {
        m_handledPaths.push_back(std::string(path));
    }
    VRPNMultiserverData &m_data;
    std::vector<std::string> m_handledPaths;
};

OSVR_PLUGIN(org_opengoggles_bundled_Multiserver) {
    osvr::pluginkit::PluginContext context(ctx);

    VRPNMultiserverData &data =
        *context.registerObjectForDeletion(new VRPNMultiserverData);
    context.registerHardwareDetectCallback(new VRPNHardwareDetect(data));

    osvrRegisterDriverInstantiationCallback(
        ctx, "YEI_3Space_Sensor", &wrappedConstructor<&createYEI>, &data);

    return OSVR_RETURN_SUCCESS;
}
