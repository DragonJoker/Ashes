project( D3D11Renderer )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.c
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cc
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_NVS_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.natvis
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.h
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hh
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inl
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inc
)

set( ${PROJECT_NAME}_USE_SPIRV_CROSS 0 )

if ( ASHES_BUILD_SPIRV_CROSS )
	set( ${PROJECT_NAME}_USE_SPIRV_CROSS 1 )
	set( ${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES
		${SPIRV_CROSS_DIR}/spirv_hlsl.cpp
	)

	set( ${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES
		${SPIRV_CROSS_DIR}/spirv_hlsl.hpp
	)
endif ()

if ( EXISTS ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/inc/ )
	set( AGS_INCLUDE_DIR ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/inc )
	set( TARGET_CXX_DEFINITIONS
		${TARGET_CXX_DEFINITIONS}
		ASHES_D3D11_USE_AMD_AGS
	)

	if ( MSVC )
		if ( MSVC15 )
			set( AGS_LIBRARY
				debug ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2015_MDd.lib
				optimized ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2015_MD.lib
			)
		else ()
			set( AGS_LIBRARY
				debug ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2017_MDd.lib
				optimized ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2017_MD.lib
			)
		endif ()
	else ()
		set( AGS_LIBRARY ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64.lib )
	endif ()
endif ()

find_package( NVAPI )

if ( NVAPI_FOUND )
	set( TARGET_CXX_DEFINITIONS
		${TARGET_CXX_DEFINITIONS}
		ASHES_D3D11_USE_NVAPI
	)
	set( TARGET_INCLUDE_DIRS
		${TARGET_INCLUDE_DIRS}
		${NVAPI_INCLUDE_DIR}
	)
	set( Ashes_BINARY_LIBRARIES
		${Ashes_BINARY_LIBRARIES}
		${NVAPI_LIBRARY}
	)
endif ()

add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_NVS_FILES}
	${${PROJECT_NAME}_CONFIG_HEADER}
	${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
)
add_library( ashes::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Renderer"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_SOVERSION}
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${PROJECT_NAME}_USE_SPIRV_CROSS=${${PROJECT_NAME}_USE_SPIRV_CROSS}
	${PROJECT_NAME}_EXPORTS
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
target_compile_features( ${PROJECT_NAME} PRIVATE
	cxx_std_17
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${Vulkan_INCLUDE_DIR}
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${SPIRV_CROSS_DIR}
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
	${AGS_INCLUDE_DIR}
)
add_target_precompiled_header_ex( ${PROJECT_NAME}
	ashes/renderer
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/${PROJECT_NAME}Pch.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/${PROJECT_NAME}Pch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	ashes::RendererCommon
	d3d11
	dxgi
	d3dcompiler
	${AGS_LIBRARY}
)

parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Buffer "Buffer" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Command "Command" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Command/Commands "Command\\\\Commands" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Core "Core" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Descriptor "Descriptor" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Enum "Enum" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Image "Image" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Miscellaneous "Miscellaneous" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Pipeline "Pipeline" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer RenderPass "RenderPass" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Shader "Shader" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Sync "Sync" )

source_group( "Header Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES} )
source_group( "Source Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES} )
source_group( "Natvis Files" FILES ${${PROJECT_NAME}_NVS_FILES} )

set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
