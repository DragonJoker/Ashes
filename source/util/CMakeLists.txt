project( util )

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/external/glslang
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

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Common" )

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

target_link_libraries( ${PROJECT_NAME}
	common
	ashes
	ashespp
	glslang
	OGLCompiler
	OSDependent
	SPVRemapper
	SPIRV
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