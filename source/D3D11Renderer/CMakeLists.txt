project( D3D11Renderer )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

set( ${PROJECT_NAME}_CONFIG_HEADER
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.hpp
)

configure_file(
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Config.hpp.in
	${${PROJECT_NAME}_CONFIG_HEADER}
	NEWLINE_STYLE LF
)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${SPIRV_CROSS_DIR}
	${ASHES_SOURCE_DIR}/include
	${ASHES_SOURCE_DIR}/include/Ashes
	${ASHES_SOURCE_DIR}/include/PlatformUtils
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}
)

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inc
)

set( ${PROJECT_NAME}_USE_SPIRV_CROSS 0 )

if ( EXISTS ${SPIRV_CROSS_DIR}/spirv_cross.cpp )
	set( ${PROJECT_NAME}_USE_SPIRV_CROSS 1 )
	set( ${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES
		${SPIRV_CROSS_DIR}/spirv_cross.cpp
		${SPIRV_CROSS_DIR}/spirv_cross_parsed_ir.cpp
		${SPIRV_CROSS_DIR}/spirv_cfg.cpp
		${SPIRV_CROSS_DIR}/spirv_glsl.cpp
		${SPIRV_CROSS_DIR}/spirv_hlsl.cpp
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
		${SPIRV_CROSS_DIR}/spirv_hlsl.hpp
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
		${${PROJECT_NAME}_CONFIG_HEADER}
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
		${${PROJECT_NAME}_CONFIG_HEADER}
		${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
		${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
	)
	set_target_properties( ${PROJECT_NAME}
		PROPERTIES
			VERSION ${PROJECT_VERSION}
			SOVERSION ${PROJECT_SOVERSION}
	)
endif ()

add_target_precompiled_header( ${PROJECT_NAME}
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Pch.hpp
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/${PROJECT_NAME}Pch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
)

target_link_libraries( ${PROJECT_NAME}
	Ashes
	PlatformUtils
	${BinLibraries}
	d3d11
	dxgi
	d3dcompiler
)

add_dependencies( ${PROJECT_NAME}
	Ashes
	PlatformUtils
)

parse_subdir_files( ${ASHES_SOURCE_DIR} Buffer "Buffer" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Command "Command" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Command/Commands "Command\\\\Commands" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Core "Core" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Descriptor "Descriptor" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Enum "Enum" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Image "Image" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Miscellaneous "Miscellaneous" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Pipeline "Pipeline" )
parse_subdir_files( ${ASHES_SOURCE_DIR} RenderPass "RenderPass" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Shader "Shader" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Sync "Sync" )
parse_subdir_files( ${ASHES_SOURCE_DIR} vulkan "vulkan" )

source_group( "Header Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES} )
source_group( "Source Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes" )

set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)

if ( ASHES_STATIC_RENDERERS )
	#We install each .lib/.a in <install_dir>/lib folder
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}
		CONFIGURATIONS Release
		EXPORT ${PROJECT_NAME}
		ARCHIVE DESTINATION lib
	)
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}_dev
		CONFIGURATIONS RelWithDebInfo
		EXPORT ${PROJECT_NAME}
		ARCHIVE DESTINATION lib/RelWithDebInfo
	)
	install(
		TARGETS ${PROJECT_NAME}
		COMPONENT ${PROJECT_NAME}_dev
		CONFIGURATIONS Debug
		EXPORT ${PROJECT_NAME}
		ARCHIVE DESTINATION lib/Debug
	)
else ()
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
	endif()
endif ()
