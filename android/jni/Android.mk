#
# Native Android build script for OSVR and osvr-server app
#

# Internal (this should be all you need to configure)
OSVR_DIR=/Users/davidteitelbaum/sensics/OGVR-Core
LIBFUNC_DIR=/Users/davidteitelbaum/sensics/libfunctionality

# Vendor prior to port
VRPN_DIR=$(OSVR_DIR)/vendor/vrpn
EIGEN_DIR=$(OSVR_DIR)/vendor/eigen-3.2.2

# 3rd party
BOOST_DIR=$(OSVR_DIR)/android/deps/Boost-for-Android/boost_1_53_0
JSON_DIR=$(OSVR_DIR)/android/deps/jsoncpp
PLUGIN_DIR=$(OSVR_DIR)/plugins/multiserver/
HIDAPI_DIR=$(OSVR_DIR)/android/deps/hid-api/hidapi
LIBUSB_DIR=$(OSVR_DIR)/android/deps/libusbx

# Build libusb
include $(CLEAR_VARS)
include $(LIBUSB_DIR)/android/jni/libusb.mk

#Build hidapi
include $(CLEAR_VARS)
include $(HIDAPI_DIR)/android/jni/Android.mk

# Build VRPN
include $(CLEAR_VARS)
LOCAL_PATH := $(VRPN_DIR)
LOCAL_MODULE    := vrpn

LOCAL_CPP_EXTENSION := .C .cpp
LOCAL_C_INCLUDES += $(VRPN_DIR)/quat

LOCAL_CFLAGS += -fpermissive -frtti
LOCAL_CFLAGS += -DVRPN_USE_HID=1

LOCAL_SRC_FILES := \
	vrpn_Analog.C \
	vrpn_Button.C \
	vrpn_BaseClass.C \
	vrpn_Connection.C \
	vrpn_FileConnection.C \
	vrpn_Serial.C \
	vrpn_Shared.C \
	vrpn_Android.C \
	vrpn_Tracker.C \
	vrpn_Tracker_RazerHydra.C \
	vrpn_HumanInterface.C \
	vrpn_YEI_3Space.C \
	vrpn_Tracker_Filter.C \
	quat/quat.c \
	quat/matrix.c \
	quat/vector.c \
	quat/xyzquat.c \
	jni_layer.cpp

include $(BUILD_STATIC_LIBRARY)

# Build libfunc
include $(CLEAR_VARS)
include $(LIBFUNC_DIR)/android/jni/Android.mk

# Include Boost Thread and System prebuilts 
# (I built these in Boost-for-Android
include $(CLEAR_VARS)
LOCAL_PATH := $(call my-dir)
LOCAL_MODULE    := boost_system-gcc-mt-1_53 
LOCAL_SRC_FILES := $(OSVR_DIR)/android/jni/$(TARGET_ARCH_ABI)/libboost_system-gcc-mt-1_53.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH := $(call my-dir)
LOCAL_MODULE    := boost_thread-gcc-mt-1_53
LOCAL_SRC_FILES := $(OSVR_DIR)/android/jni/$(TARGET_ARCH_ABI)/libboost_thread-gcc-mt-1_53.a
include $(PREBUILT_STATIC_LIBRARY)

# Build OSVR
include $(CLEAR_VARS)
LOCAL_PATH := $(call my-dir)
LOCAL_PATH := $(OSVR_DIR)/src/

LOCAL_CPP_FEATURES += exceptions rtti

LOCAL_CFLAGS += -DGXX_EXPERIMENTAL_CXX0X
LOCAL_CFLAGS += -D_STLP_USE_BOOST_SUPPORT
LOCAL_CFLAGS += -stdlib=libc++
LOCAL_CFLAGS += -fpermissive -frtti
LOCAL_CFLAGS += -DVRPN_USE_HID=1

LOCAL_C_INCLUDES := $(OSVR_DIR)/inc/
LOCAL_C_INCLUDES += $(OSVR_DIR)/src/
LOCAL_C_INCLUDES += $(VRPN_DIR)
LOCAL_C_INCLUDES += $(VRPN_DIR)/quat
LOCAL_C_INCLUDES += $(EIGEN_DIR)
LOCAL_C_INCLUDES += $(BOOST_DIR)
LOCAL_C_INCLUDES += $(OSVR_DIR)/vendor/util-headers/
LOCAL_C_INCLUDES += $(LIBFUNC_DIR)/inc/

LOCAL_STATIC_LIBRARIES += functionality
LOCAL_STATIC_LIBRARIES += vrpn
LOCAL_STATIC_LIBRARIES += boost_system-gcc-mt-1_53
LOCAL_STATIC_LIBRARIES += boost_thread-gcc-mt-1_53

LOCAL_MODULE    := osvr
LOCAL_CPP_EXTENSION := .C .cpp
LOCAL_SRC_FILES := \
	./osvr/Client/ClientContext.cpp \
	./osvr/Client/ClientInterface.cpp \
	./osvr/Client/CreateContext.cpp \
	./osvr/Client/VRPNContext.cpp \
	./osvr/ClientKit/ClientKitC.cpp \
	./osvr/ClientKit/ContextC.cpp \
	./osvr/ClientKit/InterfaceC.cpp \
	./osvr/ClientKit/InterfaceCallbackC.cpp \
	./osvr/ClientKit/ParametersC.cpp \
	./osvr/ClientKit/SystemCallbackC.cpp \
	./osvr/Connection/AsyncAccessControl.cpp \
	./osvr/Connection/AsyncDeviceToken.cpp \
	./osvr/Connection/Connection.cpp \
	./osvr/Connection/ConnectionDevice.cpp \
	./osvr/Connection/DeviceToken.cpp \
	./osvr/Connection/MessageType.cpp \
	./osvr/Connection/SyncDeviceToken.cpp \
	./osvr/Connection/VrpnBasedConnection.cpp \
	./osvr/PluginHost/PluginSpecificRegistrationContext.cpp \
	./osvr/PluginHost/PluginSpecificRegistrationContextImpl.cpp \
	./osvr/PluginHost/RegistrationContext.cpp \
	./osvr/PluginKit/AnalogInterfaceC.cpp \
	./osvr/PluginKit/ButtonInterfaceC.cpp \
	./osvr/PluginKit/DeviceInitObject.cpp \
	./osvr/PluginKit/DeviceInterfaceC.cpp \
	./osvr/PluginKit/PluginRegistrationC.cpp \
	./osvr/PluginKit/TrackerInterfaceC.cpp \
	./osvr/Routing/AddDevice.cpp \
	./osvr/Routing/Constants.cpp \
	./osvr/Routing/PathElementTools.cpp \
	./osvr/Routing/PathElementTypes.cpp \
	./osvr/Routing/PathNode.cpp \
	./osvr/Routing/PathParseAndRetrieve.cpp \
	./osvr/Routing/PathTree.cpp \
	./osvr/Server/Server.cpp \
	./osvr/Server/ServerImpl.cpp \
	./osvr/Util/AnyMap.cpp \
	./osvr/VRPN/ConstructVRPNObject.cpp \
	./osvr/Util/TimeValueC.cpp

include $(BUILD_STATIC_LIBRARY)
include $(CLEAR_VARS)

#Build JSONCPP
LOCAL_PATH := $(JSON_DIR)

LOCAL_CPP_EXTENSION := .cpp 
LOCAL_CPP_FEATURES += exceptions rtti

LOCAL_MODULE := jsoncpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := \
	./src/lib_json/json_reader.cpp \
	./src/lib_json/json_value.cpp \
	./src/lib_json/json_writer.cpp

include $(BUILD_STATIC_LIBRARY)

#Build osvr_server executable!

LOCAL_PATH := $(OSVR_DIR)
include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions rtti
LOCAL_LDLIBS += -latomic  # Interesting why we need this for an executable...

LOCAL_STATIC_LIBRARIES += osvr
LOCAL_STATIC_LIBRARIES += jsoncpp

LOCAL_MODULE := osvr_server
LOCAL_C_INCLUDES := $(OSVR_DIR)/inc/
LOCAL_C_INCLUDES += $(BOOST_DIR)
LOCAL_SRC_FILES := \
	./apps/osvr_server.cpp

include $(BUILD_EXECUTABLE)  


# Build multiserver plugin .so

LOCAL_PATH := $(PLUGIN_DIR)
include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions rtti
LOCAL_LDLIBS += -latomic -llog

LOCAL_CFLAGS += -DVRPN_USE_HID=1
LOCAL_CPP_EXTENSION := .cpp 
LOCAL_CPP_FEATURES += exceptions rtti
LOCAL_MODULE := org_opengoggles_bundled_Multiserver

LOCAL_STATIC_LIBRARIES += functionality 
LOCAL_STATIC_LIBRARIES += osvr
LOCAL_STATIC_LIBRARIES += libhidapi
LOCAL_STATIC_LIBRARIES += vrpn
LOCAL_STATIC_LIBRARIES += jsoncpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(OSVR_DIR)/inc/
LOCAL_C_INCLUDES += $(BOOST_DIR)
LOCAL_C_INCLUDES += $(HIDAPI_DIR)
LOCAL_C_INCLUDES += $(LIBFUNC_DIR)/inc/
LOCAL_C_INCLUDES += $(VRPN_DIR)
LOCAL_C_INCLUDES += $(VRPN_DIR)/quat

LOCAL_SRC_FILES := \
	./org_opengoggles_bundled_Multiserver.cpp \
	./VRPNMultiserver.cpp \
	./GetVRPNConnection.cpp \
	./DevicesWithParameters.cpp

include $(BUILD_SHARED_LIBRARY)