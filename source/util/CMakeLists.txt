set( _PROJECT_NAME util )
project( ashes_${_PROJECT_NAME} )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.c
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.cc
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.cpp
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.c
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.cc
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.cpp
)
file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.h
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.hh
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.hpp
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.inl
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/*.inc
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.h
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.hh
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.hpp
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.inl
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/*.inc
)

if ( MSVC )
	set( BinLibraries
		${Ashes_BINARY_LIBRARIES}
		glslang
		OGLCompiler
		OSDependent
		SPVRemapper
		SPIRV
		PARENT_SCOPE
	)
endif ()

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${Vulkan_INCLUDE_DIR}
		${ASHES_CONFIG_DIR}/include
		${Ashes_SOURCE_DIR}/include
		${Ashes_SOURCE_DIR}/source
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/include/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}
		${ASHES_CONFIG_DIR}/source/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}
		${CMAKE_SOURCE_DIR}/external/glslang
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		glslang
		OGLCompiler
		OSDependent
		SPVRemapper
		SPIRV
		ashes::common
	PUBLIC
		ashes::ashes
		ashes::ashespp
)
target_compile_features( ${PROJECT_NAME} PUBLIC
	cxx_std_17
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Core"
)
