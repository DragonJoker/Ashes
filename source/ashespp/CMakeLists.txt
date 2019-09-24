project( ashespp )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
	${ASHES_BINARY_DIR}/include
	${ASHES_SOURCE_DIR}/include/ashes
	${ASHES_BINARY_DIR}/include/ashes
	${ASHES_CONFIG_DIR}/include
	${ASHES_SOURCE_DIR}/source/ashes
	${ASHES_CONFIG_DIR}/source
	${Vulkan_INCLUDE_DIRS}
)

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inc
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.inc
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes/Core" )
add_dependencies( ${PROJECT_NAME}
	common
	ashes
)

add_target_precompiled_header( ${PROJECT_NAME}
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Pch.hpp
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/${PROJECT_NAME}Pch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
)

parse_subdir_files( ${ASHES_SOURCE_DIR} Buffer "Buffer" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Command "Command" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Core "Core" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Descriptor "Descriptor" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Enum "Enum" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Image "Image" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Miscellaneous "Miscellaneous" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Pipeline "Pipeline" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Core/Platform "Core\\\\\\\\Platform" )
parse_subdir_files( ${ASHES_SOURCE_DIR} RenderPass "RenderPass" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Shader "Shader" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Sync "Sync" )
parse_subdir_files( ${ASHES_SOURCE_DIR} Utils "Utils" )

if ( BUILD_ASHES )
	target_add_doc( ${PROJECT_NAME} "" "*.h *.hpp" )
endif ()

#We copy each lib in <install_dir>/lib folder
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
	ARCHIVE DESTINATION lib/Debug
)
#For libs, we install headers to <install_dir>/include/${PROJECT_NAME}
target_install_headers( ${PROJECT_NAME}
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME} )
