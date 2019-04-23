project( Gl3Renderer )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

find_package( OpenGL )

if ( OpenGL_FOUND )
	include_directories(
		${CMAKE_CURRENT_BINARY_DIR}
		${SPIRV_CROSS_DIR}
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.c
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cc
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cpp
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
		set( ${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES
			${SPIRV_CROSS_DIR}/spirv_cross.cpp
			${SPIRV_CROSS_DIR}/spirv_cross_parsed_ir.cpp
			${SPIRV_CROSS_DIR}/spirv_cfg.cpp
			${SPIRV_CROSS_DIR}/spirv_glsl.cpp
			${SPIRV_CROSS_DIR}/spirv_parser.cpp
		)

		set( ${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES
			${SPIRV_CROSS_DIR}/GLSL.std.450.h
			${SPIRV_CROSS_DIR}/spirv_common.hpp
			${SPIRV_CROSS_DIR}/spirv.hpp
			${SPIRV_CROSS_DIR}/spirv_cross.hpp
			${SPIRV_CROSS_DIR}/spirv_cross_parsed_ir.hpp
			${SPIRV_CROSS_DIR}/spirv_cfg.hpp
			${SPIRV_CROSS_DIR}/spirv_glsl.hpp
			${SPIRV_CROSS_DIR}/spirv_parser.hpp
		)
	endif ()

	add_definitions(
		-D${PROJECT_NAME}_USE_SPIRV_CROSS=${${PROJECT_NAME}_USE_SPIRV_CROSS}
	)

	if ( ASHES_STATIC_RENDERERS )
		add_definitions(
			-D${PROJECT_NAME}_STATIC
		)
		add_library( ${PROJECT_NAME} STATIC
			${${PROJECT_NAME}_SRC_FILES}
			${${PROJECT_NAME}_HDR_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
		)
	else ()
		add_definitions(
			-D${PROJECT_NAME}_EXPORTS
		)
		add_library( ${PROJECT_NAME} SHARED
			${${PROJECT_NAME}_SRC_FILES}
			${${PROJECT_NAME}_HDR_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
		)
		set_target_properties( ${PROJECT_NAME}
			PROPERTIES
				VERSION ${PROJECT_VERSION}
				SOVERSION ${PROJECT_SOVERSION}
		)
	endif ()

	add_target_precompiled_header_ex( ${PROJECT_NAME}
		ashes/renderer
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/GlRendererPch.hpp
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/GlRendererPch.cpp
		"${CMAKE_CXX_FLAGS}"
		${${PROJECT_NAME}_SRC_FILES}
	)

	target_link_libraries( ${PROJECT_NAME}
		${OPENGL_gl_LIBRARY}
		${BinLibraries}
		common
		ashes
		RendererCommon
	)

	add_dependencies( ${PROJECT_NAME}
		common
		ashes
		RendererCommon
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

	source_group( "Header Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES} )
	source_group( "Source Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES} )

	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes" )

	set( ENABLED_RENDERERS
		${ENABLED_RENDERERS}
		${PROJECT_NAME}
		PARENT_SCOPE
	)

	if ( WIN32 )
		#We install each .dll in <install_dir>/bin folder
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}
			CONFIGURATIONS Release
			EXPORT ${PROJECT_NAME}
			RUNTIME DESTINATION bin
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS RelWithDebInfo
			EXPORT ${PROJECT_NAME}
			RUNTIME DESTINATION bin/RelWithDebInfo
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS Debug
			EXPORT ${PROJECT_NAME}
			RUNTIME DESTINATION bin/Debug
		)
	else ()
		#We install each .so in <install_dir>/lib folder
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}
			CONFIGURATIONS Release
			EXPORT ${PROJECT_NAME}
			LIBRARY DESTINATION lib
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS RelWithDebInfo
			EXPORT ${PROJECT_NAME}
			LIBRARY DESTINATION lib/RelWithDebInfo
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS Debug
			LIBRARY DESTINATION lib/Debug
		)
	endif ()
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
