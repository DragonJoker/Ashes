# Install script for directory: C:/Users/Naalf/Documents/Ashes/source/ashes/common

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Naalf/Documents/Ashes/package/Ashes")
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
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Ashes" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ashes/common" TYPE FILE FILES
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/ArrayView.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/BuddyAllocator.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/BuddyAllocator.inl"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/ConstantFormat.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/DeviceAllocator.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/DynamicLibrary.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/Exception.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/FileUtils.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/FlagCombination.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/Format.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/Hash.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/Optional.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/Signal.hpp"
      "C:/Users/Naalf/Documents/Ashes/include/ashes/common/VkTypeTraits.hpp"
      )
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/Naalf/Documents/Ashes/source/ashes/common/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
