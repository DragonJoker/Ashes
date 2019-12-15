project( Test-00-Common )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( SOURCE_FILES
	Src/Application.cpp
	Src/Camera.cpp
	Src/FileUtils.cpp
	Src/MainFrame.cpp
	Src/ObjLoader.cpp
	Src/Prerequisites.cpp
)

file( GLOB HEADER_FILES
	Src/Application.hpp
	Src/Camera.hpp
	Src/FileUtils.hpp
	Src/MainFrame.hpp
	Src/ObjLoader.hpp
	Src/Prerequisites.hpp
)

if ( APPLE )
	set( SOURCE_FILES
		${SOURCE_FILES}
		Src/MetalLayer.mm
	)
	set( HEADER_FILES
		${HEADER_FILES}
		Src/MetalLayer.h
	)
endif ()

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
	${BinLibraries}
	${wxWidgets_LIBRARIES}
	${GTK2_LIBRARIES}
	ashes::common
	ashes::ashes
	ashes::ashespp
	ashes::util
)
target_include_directories( ${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/Src
	${ASHES_SOURCE_DIR}/util/Src
	${ASHES_BINARY_DIR}/util/Src
	${ASHES_SOURCE_DIR}/external/gli
	${ASHES_SOURCE_DIR}/external/glm
	${ASHES_SOURCE_DIR}/external/imgui
	${ASHES_SOURCE_DIR}/external/imgui
	${ASHES_SOURCE_DIR}/test/00-Common/Src
	${wxWidgets_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${TARGET_INCLUDE_DIRS}
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${TARGET_CXX_DEFINITIONS}
	${TARGET_DEFINITIONS}
	${wxWidgets_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PUBLIC
	${TARGET_CXX_OPTIONS}
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
