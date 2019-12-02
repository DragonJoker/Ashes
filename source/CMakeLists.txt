get_filename_component( ASHES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY )
get_filename_component( ASHES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY )

set( TARGET_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
	${ASHES_BINARY_DIR}/include
	${Vulkan_INCLUDE_DIRS}
)

if ( WIN32 )
	set( BinLibraries
		Dbghelp
	)
	if ( MSVC )
		set( TARGET_CXX_DEFINITIONS
			-D_ENFORCE_MATCHING_ALLOCATORS=0
			-DNOMINMAX
			# -DWIN32_LEAN_AND_MEAN
		)
	endif ()
else ()
	set( BinLibraries
		dl
	)
endif ()

add_subdirectory( ashes/common )

add_subdirectory( ashes )
set( ENABLED_RENDERERS ${ENABLED_RENDERERS} PARENT_SCOPE )

add_subdirectory( ashespp )

if ( ASHES_BUILD_TESTS OR ASHES_BUILD_SAMPLES )
	add_subdirectory( util )
endif ()

set( BinLibraries ${BinLibraries} PARENT_SCOPE )

install(
	TARGETS ashes ashes_common ashespp ${ENABLED_RENDERERS}
	EXPORT Ashes
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include
)
install( EXPORT Ashes
	FILE AshesConfig.cmake
	NAMESPACE ashes::
	DESTINATION lib/cmake/Ashes
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file( AshesConfigVersion.cmake
	VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}
	COMPATIBILITY AnyNewerVersion
)
