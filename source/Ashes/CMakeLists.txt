project( Ashes )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
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

set( ASHES_WIN32 0 )
set( ASHES_XCB 0 )
set( ASHES_XLIB 0 )
set( ASHES_ANDROID 0 )
set( ASHES_MIR 0 )
set( ASHES_WAYLAND 0 )

if ( WIN32 )
	set( ASHES_WIN32 1 )
elseif ( ANDROID )
	set( ASHES_ANDROID 1 )
else ()
	set( ASHES_XLIB 1 )
endif ()

set( ${PROJECT_NAME}_CONFIG_HEADER
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.hpp
)

configure_file(
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Config.hpp.in
	${${PROJECT_NAME}_CONFIG_HEADER}
	NEWLINE_STYLE LF
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_CONFIG_HEADER}
)

add_target_precompiled_header( ${PROJECT_NAME}
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Pch.hpp
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/${PROJECT_NAME}Pch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
)

parse_subdir_files( Buffer "Buffer" )
parse_subdir_files( Command "Command" )
parse_subdir_files( Core "Core" )
parse_subdir_files( Descriptor "Descriptor" )
parse_subdir_files( Enum "Enum" )
parse_subdir_files( Image "Image" )
parse_subdir_files( Miscellaneous "Miscellaneous" )
parse_subdir_files( Pipeline "Pipeline" )
parse_subdir_files( RenderPass "RenderPass" )
parse_subdir_files( Shader "Shader" )
parse_subdir_files( Sync "Sync" )
parse_subdir_files( Utils "Utils" )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes" )

# if ( BUILD_ASHES )
	# target_add_doc( ${PROJECT_NAME} "French" "*.h *.hpp" )
	# target_add_doc( ${PROJECT_NAME} "English" "*.h *.hpp" )
# endif ()

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
target_install_headers( ${PROJECT_NAME} ../../include/${PROJECT_NAME} )
