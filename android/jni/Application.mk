APP_MODULES      := vrpn libusb1.0 libhidapi functionality osvr jsoncpp osvr_server org_opengoggles_bundled_Multiserver 

NDK_TOOLCHAIN_VERSION := clang
APP_CPPFLAGS += -std=c++11
APP_STL := gnustl_static