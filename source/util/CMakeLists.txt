project( util )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.cpp
)
file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/source/${PROJECT_NAME}/*.inc
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/include/${PROJECT_NAME}/*.inc
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
target_include_directories( ${PROJECT_NAME} INTERFACE
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/external/glslang
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	common
	ashes
	ashespp
	glslang
	OGLCompiler
	OSDependent
	SPVRemapper
	SPIRV
)
target_compile_features( ${PROJECT_NAME} INTERFACE
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
	FOLDER "Common"
)

#We copy each lib in <install_dir>/lib folder
install(
	TARGETS ${PROJECT_NAME}
	COMPONENT ${PROJECT_NAME}
	CONFIGURATIONS Release
	EXPORT ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
install(
	TARGETS ${PROJECT_NAME}
	COMPONENT ${PROJECT_NAME}_dev
	CONFIGURATIONS RelWithDebInfo
	EXPORT ${PROJECT_NAME}
	RUNTIME DESTINATION bin/RelWithDebInfo
	ARCHIVE DESTINATION lib/RelWithDebInfo
	LIBRARY DESTINATION lib/RelWithDebInfo
)
install(
	TARGETS ${PROJECT_NAME}
	COMPONENT ${PROJECT_NAME}_dev
	CONFIGURATIONS Debug
	EXPORT ${PROJECT_NAME}
	RUNTIME DESTINATION bin/Debug
	ARCHIVE DESTINATION lib/Debug
	LIBRARY DESTINATION lib/Debug
)

#For libs, we install headers to <install_dir>/include/${PROJECT_NAME}
target_install_headers( ${PROJECT_NAME} ../../include/${PROJECT_NAME} )

