project( RendererCommon )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

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

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
)
target_compile_features( ${PROJECT_NAME} PUBLIC
	cxx_std_17
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "Ashes/Renderer"
)
add_dependencies( ${PROJECT_NAME}
	ashes
	common
)

parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Helper "Helper" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Layer "Layer" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Linux "Linux" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Platform "Platform" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Util "Util" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Win32 "Win32" )
