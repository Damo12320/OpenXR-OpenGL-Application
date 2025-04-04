# Install script for directory: E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/include/openxr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/OPENXR")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openxr" TYPE FILE FILES
    "E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/build/include/openxr/openxr_platform_defines.h"
    "E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/build/include/openxr/openxr.h"
    "E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/build/include/openxr/openxr_loader_negotiation.h"
    "E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/build/include/openxr/openxr_platform.h"
    "E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/build/include/openxr/openxr_reflection.h"
    "E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/build/include/openxr/openxr_reflection_structs.h"
    "E:/Downloads/OpenXR-SDK-Source-release-1.0.34/OpenXR-SDK-Source-release-1.0.34/build/include/openxr/openxr_reflection_parent_structs.h"
    )
endif()

