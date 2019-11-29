project( common )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

set( TARGET_INCLUDE_DIRS
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${ASHES_SOURCE_DIR}/include
	${ASHES_SOURCE_DIR}/include/ashes
	${ASHES_SOURCE_DIR}/include/ashes/${PROJECT_NAME}
)

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.c
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.cc
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.cpp
)

file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
	${ASHES_SOURCE_DIR}/include/ashes/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/include/ashes/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/include/ashes/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/include/ashes/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/include/ashes/${PROJECT_NAME}/*.inc
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.h
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.hh
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.hpp
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.inl
	${ASHES_SOURCE_DIR}/source/ashes/${PROJECT_NAME}/*.inc
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
	${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
)
target_link_libraries( ${PROJECT_NAME} PUBLIC
	${BinLibraries}
)
target_include_directories( ${PROJECT_NAME} PUBLIC
	${TARGET_INCLUDE_DIRS}
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
	FOLDER "Ashes/Core" )

parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes Platform "Platform" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes Win32 "Win32" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes Linux "Linux" )
parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes MacOS "MacOS" )

#We install each lib in <install_dir>/lib folder
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
target_install_dir_headers( ${PROJECT_NAME}
	${ASHES_SOURCE_DIR}/include/ashes/${PROJECT_NAME}
	ashes/${PROJECT_NAME}
)
