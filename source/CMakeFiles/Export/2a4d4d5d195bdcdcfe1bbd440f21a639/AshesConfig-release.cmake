#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ashes::ashes" for configuration "Release"
set_property(TARGET ashes::ashes APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ashes::ashes PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/ashes-1.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/ashes-1.dll"
  )

list(APPEND _cmake_import_check_targets ashes::ashes )
list(APPEND _cmake_import_check_files_for_ashes::ashes "${_IMPORT_PREFIX}/lib/ashes-1.lib" "${_IMPORT_PREFIX}/bin/ashes-1.dll" )

# Import target "ashes::ashesCommon" for configuration "Release"
set_property(TARGET ashes::ashesCommon APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ashes::ashesCommon PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/ashesCommon.lib"
  )

list(APPEND _cmake_import_check_targets ashes::ashesCommon )
list(APPEND _cmake_import_check_files_for_ashes::ashesCommon "${_IMPORT_PREFIX}/lib/ashesCommon.lib" )

# Import target "ashes::ashespp" for configuration "Release"
set_property(TARGET ashes::ashespp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ashes::ashespp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/ashespp.lib"
  )

list(APPEND _cmake_import_check_targets ashes::ashespp )
list(APPEND _cmake_import_check_files_for_ashes::ashespp "${_IMPORT_PREFIX}/lib/ashespp.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
