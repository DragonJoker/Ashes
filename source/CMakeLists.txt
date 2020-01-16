set( ASHES_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR} )

if ( ANDROID OR CMAKE_COMPILER_IS_GNUCXX )
	set( TARGET_COMPILE_DEFINITIONS
		-fdiagnostics-color=auto
		-fPIC
	)
elseif ( MSVC )
	set( TARGET_COMPILE_DEFINITIONS
		-D_CRT_SECURE_NO_WARNINGS
		-D_SCL_SECURE_NO_WARNINGS
		-D_ENFORCE_MATCHING_ALLOCATORS=0
	)
endif ()

add_subdirectory( ashes/common )

add_subdirectory( ashes )
set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )

add_subdirectory( ashespp )

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_SAMPLES )
	add_subdirectory( util )
endif ()

set( Ashes_BINARY_LIBRARIES ${Ashes_BINARY_LIBRARIES} PARENT_SCOPE )

install(
	TARGETS ashes ashes_common ashespp ${ENABLED_RENDERERS}
	EXPORT Ashes
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
install( EXPORT Ashes
	FILE AshesConfig.cmake
	NAMESPACE ashes::
	DESTINATION share/ashes/cmake
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file( AshesConfigVersion.cmake
	VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}
	COMPATIBILITY AnyNewerVersion
)
