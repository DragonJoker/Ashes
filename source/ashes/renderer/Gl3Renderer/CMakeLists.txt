project( Gl3Renderer )

if ( ${CMAKE_VERSION} VERSION_GREATER "3.10.2" ) 
	cmake_policy( SET CMP0072 NEW )
endif()

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

find_package( OpenGL )

if ( OpenGL_FOUND )
	set( BinLibraries
		${BinLibraries}
		GlRendererCommon
	)

	if ( WIN32 )
		set( BinLibraries
			${BinLibraries}
			OpenGL::GL
		)
	else ()
		find_package( XCB REQUIRED COMPONENTS xcb )
		set( BinLibraries
			${BinLibraries}
			${XCB_LIBRARIES}
			X11-xcb
			OpenGL::OpenGL
			OpenGL::EGL
			OpenGL::GLX
			X11

		)
	endif ()

	file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.c
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cc
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cpp
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_NVS_FILES
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.natvis
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.h
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hh
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hpp
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inl
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inc
	)

	set( ${PROJECT_NAME}_USE_SPIRV_CROSS 0 )

	if ( EXISTS ${SPIRV_CROSS_DIR}/spirv_cross.cpp )
		set( ${PROJECT_NAME}_USE_SPIRV_CROSS 1 )
	endif ()

	add_library( ${PROJECT_NAME} SHARED
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_NVS_FILES}
		${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
		${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
	)
	add_library( ashes::${PROJECT_NAME}
		ALIAS
		${PROJECT_NAME}
	)
	target_include_directories( ${PROJECT_NAME} PRIVATE
		${Vulkan_INCLUDE_DIRS}
		${TARGET_INCLUDE_DIRS}
		${CMAKE_CURRENT_BINARY_DIR}
		${SPIRV_CROSS_DIR}
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
	)
	target_link_libraries( ${PROJECT_NAME} PRIVATE
		ashes::GlRendererCommon
	)
	target_compile_features( ${PROJECT_NAME} PRIVATE
		cxx_std_17
	)
	target_compile_definitions( ${PROJECT_NAME} PRIVATE
		${PROJECT_NAME}_USE_SPIRV_CROSS=${${PROJECT_NAME}_USE_SPIRV_CROSS}
		${PROJECT_NAME}_EXPORTS
		${TARGET_CXX_DEFINITIONS}
	)
	target_compile_options( ${PROJECT_NAME} PRIVATE
		${TARGET_CXX_OPTIONS}
	)
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "${ASHES_BASE_DIR}/Renderer"
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_SOVERSION}
	)
	add_target_precompiled_header_ex( ${PROJECT_NAME}
		ashes/renderer
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/GlRendererPch.hpp
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/GlRendererPch.cpp
		"${CMAKE_CXX_FLAGS}"
		${${PROJECT_NAME}_SRC_FILES}
	)

	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Buffer "Buffer" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Command "Command" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Command/Commands "Command\\\\Commands" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Core "Core" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Descriptor "Descriptor" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Enum "Enum" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Image "Image" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Miscellaneous "Miscellaneous" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Pipeline "Pipeline" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer RenderPass "RenderPass" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Shader "Shader" )
	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Sync "Sync" )

	source_group( "Natvis Files" FILES ${${PROJECT_NAME}_NVS_FILES} )

	set( ENABLED_RENDERERS
		${ENABLED_RENDERERS}
		${PROJECT_NAME}
		PARENT_SCOPE
	)
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
