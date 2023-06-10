project( Test-00-Common )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/Application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/Camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/FileUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/MainFrame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/ObjLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/Prerequisites.cpp
)

file( GLOB HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/Application.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/Camera.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/FileUtils.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/MainFrame.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/ObjLoader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/Prerequisites.hpp
)

if ( APPLE )
	set( SOURCE_FILES
		${SOURCE_FILES}
		${CMAKE_CURRENT_SOURCE_DIR}/Src/MetalLayer.mm
	)
	set( HEADER_FILES
		${HEADER_FILES}
		${CMAKE_CURRENT_SOURCE_DIR}/Src/MetalLayer.h
	)
endif ()

set( PROJECTS_USE_PRECOMPILED_HEADERS OFF )

add_library( ${PROJECT_NAME} STATIC
	${SOURCE_FILES}
	${HEADER_FILES}
)
add_library( ashes::test::Common
	ALIAS
	${PROJECT_NAME}
)
add_dependencies( ${PROJECT_NAME}
	${ENABLED_RENDERERS}
)
target_link_libraries( ${PROJECT_NAME} PUBLIC
	${Ashes_BINARY_LIBRARIES}
	${wxWidgets_LIBRARIES}
	${GTK_LIBRARIES}
	${Vulkan_LIBRARY}
	ashes::common
	ashes::ashespp
	ashes::util
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Src
)
target_include_directories( ${PROJECT_NAME}
	SYSTEM PUBLIC
		${Ashes_SOURCE_DIR}/external/gli
		${Ashes_SOURCE_DIR}/external/glm
		${Ashes_SOURCE_DIR}/external/imgui
		${Ashes_SOURCE_DIR}/external/imgui
		${wxWidgets_INCLUDE_DIRS}
		${GTK_INCLUDE_DIRS}
		${TARGET_INCLUDE_DIRS}
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${TARGET_CXX_DEFINITIONS}
	${Ashes_BINARY_DEFINITIONS}
	${wxWidgets_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME}
	PUBLIC
		${TARGET_CXX_OPTIONS}
)
set_property( TARGET ${PROJECT_NAME}
	PROPERTY
		FOLDER "${Ashes_BASE_DIR}/Utils"
)
set( ASSETS_FOLDER ${CMAKE_SOURCE_DIR}/test/Assets )

file( GLOB_RECURSE ASSETS_FILES
	${ASSETS_FOLDER}/*
)

foreach( ASSET ${ASSETS_FILES} )
	file( RELATIVE_PATH REL_ASSET ${ASSETS_FOLDER} ${ASSET} )
	get_filename_component( FOLDER ${REL_ASSET} DIRECTORY )
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/Assets/${FOLDER}>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/Assets/${FOLDER}>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/Assets/${FOLDER}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSETS_FOLDER}/${REL_ASSET}
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/Assets/${FOLDER}>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/Assets/${FOLDER}>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/Assets/${FOLDER}>
	)
endforeach()
