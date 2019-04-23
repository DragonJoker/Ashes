project( RendererCommon )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
	${ASHES_SOURCE_DIR}/include/renderer/${PROJECT_NAME}
)

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${ASHES_SOURCE_DIR}/include/renderer/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/include/renderer/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/include/renderer/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/include/renderer/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/include/renderer/${PROJECT_NAME}/*.inc
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/source/renderer/${PROJECT_NAME}/*.inc
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
)

add_dependencies( ${PROJECT_NAME}
	ashes
	common
)

parse_subdir_files_ex( ${ASHES_SOURCE_DIR} renderer Helper "Helper" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} renderer Layer "Layer" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} renderer Linux "Linux" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} renderer Platform "Platform" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} renderer Util "Util" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} renderer Win32 "Win32" )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes/Renderer" )

#We install each lib in <install_dir>/lib folder
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
