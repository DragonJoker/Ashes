project( TestRenderer )

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

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.h
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hh
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inl
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inc
)

add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${Vulkan_INCLUDE_DIR}
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	ashes::RendererCommon
)
target_compile_features( ${PROJECT_NAME} PRIVATE
	cxx_std_17
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${PROJECT_NAME}_EXPORTS
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Renderer"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_SOVERSION}
)
add_target_precompiled_header_ex( ${PROJECT_NAME}
	ashes/renderer
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/${PROJECT_NAME}Pch.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/${PROJECT_NAME}Pch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
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

set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
