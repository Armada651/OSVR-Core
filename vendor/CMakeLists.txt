if(BUILD_TESTING)
    if(WIN32)
        set(gtest_force_shared_crt ON)
    endif()
    add_subdirectory(gtest-1.7.0)
endif()
set(BUILD_SHARED_LIBS OFF)

if(NOT (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/vrpn_Connection.h" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/submodules/hidapi/hidapi/hidapi.h"))
	message(FATAL_ERROR "You are missing the VRPN git submodule or one of its submodules- you must have not done a recursive clone. You can get it now by running git submodule update --init --recursive from the root source directory.")
endif()

# Build VRPN as subproject
set(VRPN_GPL_SERVER FALSE CACHE INTERNAL "" FORCE)
set(VRPN_USE_JSONNET TRUE CACHE INTERNAL "" FORCE)
set(VRPN_USE_LOCAL_JSONCPP TRUE CACHE INTERNAL "" FORCE)
if(BUILD_CLIENT AND NOT BUILD_SERVER)
    set(VRPN_BUILD_SERVER_LIBRARY FALSE CACHE INTERNAL "" FORCE)
    set(VRPN_BUILD_CLIENT_LIBRARY TRUE CACHE INTERNAL "" FORCE)
else()
    set(VRPN_BUILD_SERVER_LIBRARY TRUE CACHE INTERNAL "" FORCE)
    set(VRPN_BUILD_CLIENT_LIBRARY FALSE CACHE INTERNAL "" FORCE)
    set(VRPN_USE_HID TRUE CACHE INTERNAL "" FORCE)
    set(VRPN_USE_LOCAL_HIDAPI TRUE CACHE INTERNAL "" FORCE)
    if(NOT WIN32)
        set(VRPN_USE_LIBUSB_1_0 TRUE CACHE INTERNAL "" FORCE)
    endif()
endif()
add_subdirectory(vrpn)

# Create an interface target to more easily consume VRPN internally.
add_library(vendored-vrpn INTERFACE)
if(BUILD_CLIENT AND NOT BUILD_SERVER)
    target_link_libraries(vendored-vrpn INTERFACE ${VRPN_CLIENT_LIBRARY})
else()
    target_link_libraries(vendored-vrpn INTERFACE ${VRPN_SERVER_LIBRARY})
endif()
target_include_directories(vendored-vrpn INTERFACE ${VRPN_INCLUDE_DIRS})
target_compile_definitions(vendored-vrpn INTERFACE ${VRPN_DEFINITIONS})

# Interface target for hidapi compiled as part of VRPN.
add_library(vendored-hidapi INTERFACE)
target_link_libraries(vendored-hidapi INTERFACE vendored-vrpn)
if(VRPN_HIDAPI_SOURCE_ROOT)
    target_include_directories(vendored-hidapi INTERFACE "${VRPN_HIDAPI_SOURCE_ROOT}")
else()
    target_include_directories(vendored-hidapi INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/submodules/hidapi")
endif()

# Interface target for jsoncpp compiled as part of VRPN.
add_library(jsoncpp_lib INTERFACE)
target_link_libraries(jsoncpp_lib INTERFACE vendored-vrpn)
if(JSONCPP_INCLUDE_DIRS)
    target_include_directories(jsoncpp_lib INTERFACE "${JSONCPP_INCLUDE_DIRS}")
else()
    target_include_directories(jsoncpp_lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/submodules/jsoncpp/include")
endif()

# Interface target for util headers
add_library(util-headers INTERFACE)
target_include_directories(util-headers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/util-headers")

# Interface target for the run loop manager in util headers
add_library(util-runloopmanager INTERFACE)
target_include_directories(util-runloopmanager INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/util-headers")
target_link_libraries(util-runloopmanager INTERFACE boost_thread)

# Interface target for intrusive_ptr support for COM
add_library(com_smart_pointer INTERFACE)
target_include_directories(com_smart_pointer INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/com_smart_pointer" ${Boost_INCLUDE_DIRS})

# Interface target for Eigen
# Actually a post-3.2.5 commit on the 3.2 branch: ec9c7b2e30a85212efd90937597e33515eff0c90
add_library(eigen-headers INTERFACE)
target_include_directories(eigen-headers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/eigen-3.2.5")
target_compile_definitions(eigen-headers INTERFACE EIGEN_MPL2_ONLY)

###
# ETWProviders from UIforETW
set(ETWROOT "${CMAKE_CURRENT_SOURCE_DIR}/UIforETWbins")
set(ETWPROVIDERS_FOUND "FALSE" CACHE INTERNAL "" FORCE)

if(WIN32)
    set(ETWSUFFIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ETWSUFFIX 64)
    endif()
    set(ETWPROVIDERS_RUNTIME_LIBRARY "${ETWROOT}/bin/ETWProviders${ETWSUFFIX}.dll")
    if(EXISTS "${ETWPROVIDERS_RUNTIME_LIBRARY}")
        add_library(ETWProviders INTERFACE)
        target_include_directories(ETWProviders INTERFACE "${ETWROOT}/include")
        # set_target_properties(ETWProviders PROPERTIES
        #     INTERFACE_INCLUDE_DIRECTORIES "${ETWROOT}/include")
        if(NOT MSVC)
            target_include_directories(ETWProviders INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/dummy-sal")
        endif()
        if(MINGW)
            target_link_libraries(ETWProviders INTERFACE "${ETWPROVIDERS_RUNTIME_LIBRARY}")
        else()
            target_link_libraries(ETWProviders INTERFACE "${ETWROOT}/lib/ETWProviders${ETWSUFFIX}.lib")
        endif()
        set(ETWPROVIDERS_RUNTIME_LIBRARY "${ETWPROVIDERS_RUNTIME_LIBRARY}" CACHE INTERNAL "" FORCE)
        set(ETWPROVIDERS_MANIFEST "${ETWROOT}/bin/ETWProviders.man" CACHE INTERNAL "" FORCE)
        set(ETWPROVIDERS_FOUND "TRUE" CACHE INTERNAL "" FORCE)
        # set_target_properties(ETWProviders PROPERTIES
        #     IMPORTED_IMPLIB "${ETWROOT}/lib/ETWProviders${ETWSUFFIX}.lib"
        #     IMPORTED_LOCATION "${ETWPROVIDERS_RUNTIME_LIBRARY}"
        # )
    else()
        message(STATUS "UIforETWbins submodule missing, so ETW-based tracing unavailable.")
    endif()
endif()

# ETW README
if(ETWPROVIDERS_FOUND)
    # Trigger CMake if this file changes.
    configure_file("${ETWROOT}/README.md" "${CMAKE_CURRENT_BINARY_DIR}/README.junk" COPYONLY)

    # Extract the release URL, which is always on its own line in the submodule readme.
    file(STRINGS "${CMAKE_CURRENT_BINARY_DIR}/README.junk" UIFORETW_RELEASE
        LIMIT_COUNT 1
        REGEX "^<.*>$")

    # Set the location for the generated README file, for use further up in the build system.
    set(ETWPROVIDERS_OSVR_README "${CMAKE_CURRENT_BINARY_DIR}/README-ETW.md" CACHE INTERNAL "" FORCE)

    # Actually do the generation, to put the release URL in the readme we'll distribute.
    configure_file("README-ETW.md.in" "${ETWPROVIDERS_OSVR_README}" @ONLY)
endif()
