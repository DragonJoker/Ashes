project( ${TARGET_NAME} )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( SOURCE_FILES
	Src/Application.cpp
	Src/AssimpLoader.cpp
	Src/Camera.cpp
	Src/FileUtils.cpp
	Src/Gui.cpp
	Src/MainFrame.cpp
	Src/NodesRenderer.cpp
	Src/OpaqueRendering.cpp
	Src/PrecompiledHeader.cpp
	Src/Prerequisites.cpp
	Src/RenderPanel.cpp
	Src/RenderTarget.cpp
	Src/Scene.cpp
	Src/TransparentRendering.cpp
	${Ashes_SOURCE_DIR}/external/imgui/imgui.cpp
	${Ashes_SOURCE_DIR}/external/imgui/imgui_draw.cpp
	${Ashes_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
)
file( GLOB HEADER_FILES
	Src/Application.hpp
	Src/AssimpLoader.hpp
	Src/Camera.hpp
	Src/FileUtils.hpp
	Src/Gui.hpp
	Src/MainFrame.hpp
	Src/NodesRenderer.hpp
	Src/OpaqueRendering.hpp
	Src/PrecompiledHeader.hpp
	Src/Prerequisites.hpp
	Src/RenderPanel.hpp
	Src/RenderTarget.hpp
	Src/Scene.hpp
	Src/TransparentRendering.hpp
)
file( GLOB GLSL_SHADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.vert
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.frag
)
file( GLOB SHADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.*
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

source_group( "Shader Files" FILES ${GLSL_SHADER_FILES} )

add_library( ${PROJECT_NAME} STATIC
	${SOURCE_FILES}
	${HEADER_FILES}
	${GLSL_SHADER_FILES}
)
add_library( ashes::sample::Common
	ALIAS
	${PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		${Ashes_BINARY_LIBRARIES}
	PUBLIC
		ashes::ashespp
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${Vulkan_INCLUDE_DIR}
		${Ashes_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/Src
		${Ashes_SOURCE_DIR}/external/imgui
)
add_target_precompiled_header( ${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/Src/PrecompiledHeader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/PrecompiledHeader.cpp
	"${CMAKE_CXX_FLAGS}"
	${SOURCE_FILES}
)

foreach( SHADER ${SHADER_FILES} )
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/Sample-00-Common/Shaders>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/Sample-00-Common/Shaders>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/Sample-00-Common/Shaders>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER}
			$<$<CONFIG:Debug>:${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}/share/Sample-00-Common/Shaders>
			$<$<CONFIG:Release>:${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}/share/Sample-00-Common/Shaders>
			$<$<CONFIG:RelWithDebInfo>:${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}/share/Sample-00-Common/Shaders>
	)
endforeach()

set( ASSETS_FOLDER ${CMAKE_SOURCE_DIR}/samples/Assets )

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

