#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ashes::ashesD3D11Renderer" for configuration "Debug"
set_property(TARGET ashes::ashesD3D11Renderer APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ashes::ashesD3D11Renderer PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/ashesD3D11Rendererd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/ashesD3D11Rendererd.dll"
  )

list(APPEND _cmake_import_check_targets ashes::ashesD3D11Renderer )
list(APPEND _cmake_import_check_files_for_ashes::ashesD3D11Renderer "${_IMPORT_PREFIX}/lib/ashesD3D11Rendererd.lib" "${_IMPORT_PREFIX}/bin/ashesD3D11Rendererd.dll" )

# Import target "ashes::ashesXBoxRenderer" for configuration "Debug"
set_property(TARGET ashes::ashesXBoxRenderer APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ashes::ashesXBoxRenderer PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/ashesXBoxRendererd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/ashesXBoxRendererd.dll"
  )

list(APPEND _cmake_import_check_targets ashes::ashesXBoxRenderer )
list(APPEND _cmake_import_check_files_for_ashes::ashesXBoxRenderer "${_IMPORT_PREFIX}/lib/ashesXBoxRendererd.lib" "${_IMPORT_PREFIX}/bin/ashesXBoxRendererd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
