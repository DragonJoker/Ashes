#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ashes::ashes" for configuration "Debug"
set_property(TARGET ashes::ashes APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ashes::ashes PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/ashesd-1.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/ashesd-1.dll"
  )

list(APPEND _cmake_import_check_targets ashes::ashes )
list(APPEND _cmake_import_check_files_for_ashes::ashes "${_IMPORT_PREFIX}/lib/ashesd-1.lib" "${_IMPORT_PREFIX}/bin/ashesd-1.dll" )

# Import target "ashes::ashesCommon" for configuration "Debug"
set_property(TARGET ashes::ashesCommon APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ashes::ashesCommon PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/ashesCommond.lib"
  )

list(APPEND _cmake_import_check_targets ashes::ashesCommon )
list(APPEND _cmake_import_check_files_for_ashes::ashesCommon "${_IMPORT_PREFIX}/lib/ashesCommond.lib" )

# Import target "ashes::ashespp" for configuration "Debug"
set_property(TARGET ashes::ashespp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ashes::ashespp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/ashesppd.lib"
  )

list(APPEND _cmake_import_check_targets ashes::ashespp )
list(APPEND _cmake_import_check_files_for_ashes::ashespp "${_IMPORT_PREFIX}/lib/ashesppd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
