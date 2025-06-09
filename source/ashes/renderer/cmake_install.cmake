# Install script for directory: C:/Users/Naalf/Documents/Ashes/source/ashes/renderer

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "AshesRenderers" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Debug/lib/ashesD3D11Rendererd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Release/lib/ashesD3D11Renderer.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/D3D11Renderer/MinSizeRel/ashesD3D11Renderer.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/RelWithDebInfo/lib/ashesD3D11Renderer.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "AshesRenderers" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Debug/bin/ashesD3D11Rendererd.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Release/bin/ashesD3D11Renderer.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/D3D11Renderer/MinSizeRel/ashesD3D11Renderer.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/RelWithDebInfo/bin/ashesD3D11Renderer.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "AshesRenderers" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Debug/lib/ashesXBoxRendererd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Release/lib/ashesXBoxRenderer.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/XBoxRenderer/MinSizeRel/ashesXBoxRenderer.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/RelWithDebInfo/lib/ashesXBoxRenderer.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "AshesRenderers" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Debug/bin/ashesXBoxRendererd.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/Release/bin/ashesXBoxRenderer.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/XBoxRenderer/MinSizeRel/ashesXBoxRenderer.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Naalf/Documents/Ashes/binaries/x64/RelWithDebInfo/bin/ashesXBoxRenderer.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES
      "C:/Users/Naalf/Documents/Ashes/binaries/x64/Debug/bin/ashesD3D11Rendererd.pdb"
      "C:/Users/Naalf/Documents/Ashes/binaries/x64/Debug/bin/ashesXBoxRendererd.pdb"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES
      "C:/Users/Naalf/Documents/Ashes/binaries/x64/Release/bin/ashesD3D11Renderer.pdb"
      "C:/Users/Naalf/Documents/Ashes/binaries/x64/Release/bin/ashesXBoxRenderer.pdb"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES
      "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/D3D11Renderer/MinSizeRel/ashesD3D11Renderer.pdb"
      "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/XBoxRenderer/MinSizeRel/ashesXBoxRenderer.pdb"
      )
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES
      "C:/Users/Naalf/Documents/Ashes/binaries/x64/RelWithDebInfo/bin/ashesD3D11Renderer.pdb"
      "C:/Users/Naalf/Documents/Ashes/binaries/x64/RelWithDebInfo/bin/ashesXBoxRenderer.pdb"
      )
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "AshesRenderers" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes/AshesRenderersConfig.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes/AshesRenderersConfig.cmake"
         "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/CMakeFiles/Export/2a4d4d5d195bdcdcfe1bbd440f21a639/AshesRenderersConfig.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes/AshesRenderersConfig-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes/AshesRenderersConfig.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes" TYPE FILE FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/CMakeFiles/Export/2a4d4d5d195bdcdcfe1bbd440f21a639/AshesRenderersConfig.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes" TYPE FILE FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/CMakeFiles/Export/2a4d4d5d195bdcdcfe1bbd440f21a639/AshesRenderersConfig-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes" TYPE FILE FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/CMakeFiles/Export/2a4d4d5d195bdcdcfe1bbd440f21a639/AshesRenderersConfig-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes" TYPE FILE FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/CMakeFiles/Export/2a4d4d5d195bdcdcfe1bbd440f21a639/AshesRenderersConfig-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ashes" TYPE FILE FILES "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/CMakeFiles/Export/2a4d4d5d195bdcdcfe1bbd440f21a639/AshesRenderersConfig-release.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/RendererCommon/cmake_install.cmake")
  include("C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/D3D11Renderer/cmake_install.cmake")
  include("C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/XBoxRenderer/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/Naalf/Documents/Ashes/source/ashes/renderer/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
