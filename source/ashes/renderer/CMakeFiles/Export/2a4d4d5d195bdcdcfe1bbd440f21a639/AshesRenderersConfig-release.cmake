#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ashes::ashesD3D11Renderer" for configuration "Release"
set_property(TARGET ashes::ashesD3D11Renderer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ashes::ashesD3D11Renderer PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/ashesD3D11Renderer.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/ashesD3D11Renderer.dll"
  )

list(APPEND _cmake_import_check_targets ashes::ashesD3D11Renderer )
list(APPEND _cmake_import_check_files_for_ashes::ashesD3D11Renderer "${_IMPORT_PREFIX}/lib/ashesD3D11Renderer.lib" "${_IMPORT_PREFIX}/bin/ashesD3D11Renderer.dll" )

# Import target "ashes::ashesXBoxRenderer" for configuration "Release"
set_property(TARGET ashes::ashesXBoxRenderer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ashes::ashesXBoxRenderer PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/ashesXBoxRenderer.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/ashesXBoxRenderer.dll"
  )

list(APPEND _cmake_import_check_targets ashes::ashesXBoxRenderer )
list(APPEND _cmake_import_check_files_for_ashes::ashesXBoxRenderer "${_IMPORT_PREFIX}/lib/ashesXBoxRenderer.lib" "${_IMPORT_PREFIX}/bin/ashesXBoxRenderer.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
