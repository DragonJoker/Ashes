project( ashes_util )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${ASHES_SOURCE_DIR}/source/util/*.c
	${ASHES_SOURCE_DIR}/source/util/*.cc
	${ASHES_SOURCE_DIR}/source/util/*.cpp
	${ASHES_SOURCE_DIR}/include/util/*.c
	${ASHES_SOURCE_DIR}/include/util/*.cc
	${ASHES_SOURCE_DIR}/include/util/*.cpp
)
file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${ASHES_SOURCE_DIR}/source/util/*.h
	${ASHES_SOURCE_DIR}/source/util/*.hh
	${ASHES_SOURCE_DIR}/source/util/*.hpp
	${ASHES_SOURCE_DIR}/source/util/*.inl
	${ASHES_SOURCE_DIR}/source/util/*.inc
	${ASHES_SOURCE_DIR}/include/util/*.h
	${ASHES_SOURCE_DIR}/include/util/*.hh
	${ASHES_SOURCE_DIR}/include/util/*.hpp
	${ASHES_SOURCE_DIR}/include/util/*.inl
	${ASHES_SOURCE_DIR}/include/util/*.inc
)

if ( MSVC )
	set( BinLibraries
		${BinLibraries}
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
		${Vulkan_INCLUDE_DIRS}
		${ASHES_CONFIG_DIR}/include
		${ASHES_SOURCE_DIR}/include
		${ASHES_SOURCE_DIR}/source
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/include/util
		${ASHES_SOURCE_DIR}/include/util
		${ASHES_CONFIG_DIR}/source/util
		${ASHES_SOURCE_DIR}/source/util
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
	FOLDER "${ASHES_BASE_DIR}/Core"
)
