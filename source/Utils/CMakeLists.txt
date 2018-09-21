project( Utils )

if ( NOT glslang_ROOT_DIR )
	set( glslang_ROOT_DIR ${CMAKE_SOURCE_DIR}/../dependencies/glslang )
endif ()
if ( NOT SPIRV_ROOT_DIR )
	set( SPIRV_ROOT_DIR ${CMAKE_SOURCE_DIR}/../dependencies/glslang )
endif ()

find_package( glslang )
find_package( SPIRV )

if ( glslang_FOUND AND SPIRV_FOUND )
	set( UTILS_GLSL_TO_SPV 1 )
else ()
	set( UTILS_GLSL_TO_SPV 0 )
endif ()

set( ${PROJECT_NAME}_CONFIG_HEADER
	${CMAKE_CURRENT_BINARY_DIR}/Src/UtilsConfig.hpp
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Src/UtilsConfig.hpp.in
	${${PROJECT_NAME}_CONFIG_HEADER}
	NEWLINE_STYLE LF
)

include_directories(
	${CMAKE_SOURCE_DIR}/Ashes/Ashes/Src
	${CMAKE_BINARY_DIR}/Ashes/Ashes/Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src
	${CMAKE_CURRENT_BINARY_DIR}/Src
	${glslang_INCLUDE_DIR}
	${SPIRV_INCLUDE_DIR}
)

file( GLOB ${PROJECT_NAME}_SRC_FILES
	Src/*.c
	Src/*.cc
	Src/*.cpp
)

file( GLOB ${PROJECT_NAME}_HDR_FILES
	Src/*.h
	Src/*.hh
	Src/*.hpp
	Src/*.inl
	Src/*.inc
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_CONFIG_HEADER}
)

target_link_libraries( ${PROJECT_NAME}
	${glslang_LIBRARIES}
	${SPIRV_LIBRARIES}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )

#We copy each lib in <install_dir>/lib folder
install(
	TARGETS ${PROJECT_NAME}
	COMPONENT ${PROJECT_NAME}_dev
	CONFIGURATIONS Release RelWithDebInfo
	EXPORT ${PROJECT_NAME}
	ARCHIVE DESTINATION lib/${PROJECTS_PLATFORM}/Release
)
install(
	TARGETS ${PROJECT_NAME}
	COMPONENT ${PROJECT_NAME}_dev
	CONFIGURATIONS Debug
	ARCHIVE DESTINATION lib/${PROJECTS_PLATFORM}/Debug
)

#For libs, we install headers to <install_dir>/include/${PROJECT_NAME}
target_install_headers( ${PROJECT_NAME} Src )