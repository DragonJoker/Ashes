set( ASHES_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR} )

if ( ANDROID OR CMAKE_COMPILER_IS_GNUCXX )
	set( TARGET_COMPILE_OPTIONS
		-fdiagnostics-color=auto
		-fPIC
	)
elseif ( MSVC )
	set( TARGET_COMPILE_DEFINITIONS
		_CRT_SECURE_NO_WARNINGS
		_SCL_SECURE_NO_WARNINGS
		_ENFORCE_MATCHING_ALLOCATORS=0
	)
endif ()

if ( VCPKG_TOOLCHAIN OR NOT EXISTS ${Ashes_SOURCE_DIR}/external/spirv_cross/spirv_cross.cpp )
	set( ASHES_BUILD_SPIRV_CROSS OFF )
	if ( VCPKG_TOOLCHAIN )
		find_package( spirv_cross_core CONFIG REQUIRED )
		find_package( spirv_cross_glsl CONFIG REQUIRED )
		find_package( spirv_cross_hlsl CONFIG REQUIRED )
		find_package( spirv_cross_reflect CONFIG REQUIRED )
		find_package( spirv_cross_util CONFIG REQUIRED )
		find_package( spirv_cross_cpp CONFIG REQUIRED )
		if ( spirv_cross_core_FOUND
			AND spirv_cross_glsl_FOUND
			AND spirv_cross_reflect_FOUND
			AND spirv_cross_util_FOUND
			AND spirv_cross_cpp_FOUND )
			set( spirv-cross_FOUND ON )
		endif ()
	else ()
		find_package( spirv-cross REQUIRED COMPONENTS core cpp glsl hlsl reflect util )
	endif ()
	if ( spirv-cross_FOUND )
		set( ASHES_HAS_SPIRV_CROSS ON )
		set( SPIRV_CROSS_LIBRARIES
			spirv-cross-core
			spirv-cross-cpp
			spirv-cross-glsl
			spirv-cross-reflect
			spirv-cross-util
		)
		set( SPIRV_CROSS_HLSL_LIBRARIES
			spirv-cross-hlsl
		)
	else ()
		set( ASHES_HAS_SPIRV_CROSS OFF )
		message( FATAL_ERROR "Couldn't find spirv-cross" )
	endif ()
else ()
	set( ASHES_BUILD_SPIRV_CROSS ON )
	set( ASHES_HAS_SPIRV_CROSS ON )
	set( SPIRV_CROSS_INCLUDE_DIR ${Ashes_SOURCE_DIR}/external )
	set( SPIRV_CROSS_DIR ${SPIRV_CROSS_INCLUDE_DIR}/spirv_cross )
endif ()

add_subdirectory( ashes/common )

add_subdirectory( ashes )
set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )

add_subdirectory( ashespp )

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_SAMPLES )
	add_subdirectory( util )
endif ()

set( Ashes_BINARY_LIBRARIES ${Ashes_BINARY_LIBRARIES} PARENT_SCOPE )
if ( WIN32 )
	install(
		TARGETS vulkan ashes_common ashespp ${ENABLED_RENDERERS}
		EXPORT Ashes
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)
else ()
	install(
		TARGETS vulkan ashes_common ashespp ${ENABLED_RENDERERS}
		EXPORT Ashes
		RUNTIME DESTINATION bin/Ashes
		LIBRARY DESTINATION lib/Ashes
		ARCHIVE DESTINATION lib/Ashes
	)
endif ()
install( EXPORT Ashes
	FILE AshesConfig.cmake
	NAMESPACE ashes::
	DESTINATION lib/cmake/ashes
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file( AshesConfigVersion.cmake
	VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}
	COMPATIBILITY AnyNewerVersion
)
