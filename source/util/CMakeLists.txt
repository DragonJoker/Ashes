project( ashes_util )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${Ashes_SOURCE_DIR}/source/util/*.c
	${Ashes_SOURCE_DIR}/source/util/*.cc
	${Ashes_SOURCE_DIR}/source/util/*.cpp
	${Ashes_SOURCE_DIR}/include/util/*.c
	${Ashes_SOURCE_DIR}/include/util/*.cc
	${Ashes_SOURCE_DIR}/include/util/*.cpp
)
file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/source/util/*.h
	${Ashes_SOURCE_DIR}/source/util/*.hh
	${Ashes_SOURCE_DIR}/source/util/*.hpp
	${Ashes_SOURCE_DIR}/source/util/*.inl
	${Ashes_SOURCE_DIR}/source/util/*.inc
	${Ashes_SOURCE_DIR}/include/util/*.h
	${Ashes_SOURCE_DIR}/include/util/*.hh
	${Ashes_SOURCE_DIR}/include/util/*.hpp
	${Ashes_SOURCE_DIR}/include/util/*.inl
	${Ashes_SOURCE_DIR}/include/util/*.inc
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
add_library( ashes::util
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
		${ASHES_CONFIG_DIR}/include/util
		${Ashes_SOURCE_DIR}/include/util
		${ASHES_CONFIG_DIR}/source/util
		${Ashes_SOURCE_DIR}/source/util
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
