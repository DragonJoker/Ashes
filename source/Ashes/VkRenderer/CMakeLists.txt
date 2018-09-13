project( VkRenderer )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

if ( NOT glslang_ROOT_DIR )
	set( glslang_ROOT_DIR ${CMAKE_SOURCE_DIR}/../dependencies/glslang )
endif ()
if ( NOT SPIRV_ROOT_DIR )
	set( SPIRV_ROOT_DIR ${CMAKE_SOURCE_DIR}/../dependencies/glslang )
endif ()

find_package( glslang )
find_package( SPIRV )

if ( glslang_FOUND AND SPIRV_FOUND )
	include_directories(
		${glslang_INCLUDE_DIR}
		${SPIRV_INCLUDE_DIR}
	)
	set( BinaryLibraries
		${BinaryLibraries}
		PARENT_SCOPE
	)
	set( VKRENDERER_GLSL_TO_SPV 1 )
else ()
	set( VKRENDERER_GLSL_TO_SPV 0 )
	set( SPIRV_FOUND TRUE )
	set( glslang_FOUND TRUE )
endif ()

set( ${PROJECT_NAME}_CONFIG_HEADER
	${CMAKE_CURRENT_BINARY_DIR}/Src/VkRendererConfig.hpp
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Src/VkRendererConfig.hpp.in
	${${PROJECT_NAME}_CONFIG_HEADER}
	NEWLINE_STYLE LF
)

if ( glslang_FOUND AND SPIRV_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/../Ashes/Src
		${CMAKE_CURRENT_SOURCE_DIR}/../Ashes/Src
		${CMAKE_CURRENT_SOURCE_DIR}/Src
		${CMAKE_CURRENT_BINARY_DIR}/Src
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
		Src/*.c
		Src/*.cc
		Src/*.cpp
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
		Src/*.h
		Src/*.hh
		Src/*.hpp
		Src/*.inl
		Src/*.inc
	)

	if ( ASHES_STATIC_RENDERERS )
		add_definitions(
			-DVkRenderer_STATIC
		)
		add_library( ${PROJECT_NAME} STATIC
			${${PROJECT_NAME}_SRC_FILES}
			${${PROJECT_NAME}_HDR_FILES}
			${${PROJECT_NAME}_CONFIG_HEADER}
		)
	else ()
		add_definitions(
			-DVkRenderer_EXPORTS
		)
		add_library( ${PROJECT_NAME} SHARED
			${${PROJECT_NAME}_SRC_FILES}
			${${PROJECT_NAME}_HDR_FILES}
			${${PROJECT_NAME}_CONFIG_HEADER}
		)
	endif ()

	if ( BUILD_ASHES )
		add_target_precompiled_header( ${PROJECT_NAME}
			Src/VkRendererPch.hpp
			Src/VkRendererPch.cpp
			${CMAKE_CXX_FLAGS}
			${${PROJECT_NAME}_SRC_FILES}
		)
	else ()
		add_target_precompiled_header( ${PROJECT_NAME}
			VkRendererPch.hpp
			VkRendererPch.cpp
			${CMAKE_CXX_FLAGS}
			${${PROJECT_NAME}_SRC_FILES}
		)
	endif ()

	target_link_libraries( ${PROJECT_NAME}
		${glslang_LIBRARIES}
		${SPIRV_LIBRARIES}
		${BinLibraries}
		Ashes
	)

	add_dependencies( ${PROJECT_NAME}
		Ashes
	)

	parse_subdir_files( Src/Buffer "Buffer" )
	parse_subdir_files( Src/Command "Command" )
	parse_subdir_files( Src/Core "Core" )
	parse_subdir_files( Src/Descriptor "Descriptor" )
	parse_subdir_files( Src/Enum "Enum" )
	parse_subdir_files( Src/Image "Image" )
	parse_subdir_files( Src/Miscellaneous "Miscellaneous" )
	parse_subdir_files( Src/Pipeline "Pipeline" )
	parse_subdir_files( Src/RenderPass "RenderPass" )
	parse_subdir_files( Src/Shader "Shader" )
	parse_subdir_files( Src/Sync "Sync" )
	parse_subdir_files( Src/vulkan "vulkan" )

	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes" )

	set( ENABLED_RENDERERS
		${ENABLED_RENDERERS}
		VkRenderer
		PARENT_SCOPE
	)

	if ( ASHES_STATIC_RENDERERS )
		#We install each .lib/.a in <install_dir>/lib folder
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}
			CONFIGURATIONS Release RelWithDebInfo
			EXPORT ${PROJECT_NAME}
			ARCHIVE DESTINATION lib/${PROJECTS_PLATFORM}/Release
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}
			CONFIGURATIONS Debug
			EXPORT ${PROJECT_NAME}
			ARCHIVE DESTINATION lib/${PROJECTS_PLATFORM}/Debug
		)
	else ()
		if ( WIN32 )
			#We install each .dll in <install_dir>/bin folder
			install(
				TARGETS ${PROJECT_NAME}
				COMPONENT ${PROJECT_NAME}
				CONFIGURATIONS Release RelWithDebInfo
				EXPORT ${PROJECT_NAME}
				RUNTIME DESTINATION bin/${PROJECTS_PLATFORM}/Release
			)
			install(
				TARGETS ${PROJECT_NAME}
				COMPONENT ${PROJECT_NAME}
				CONFIGURATIONS Debug
				EXPORT ${PROJECT_NAME}
				RUNTIME DESTINATION bin/${PROJECTS_PLATFORM}/Debug
			)
		else ()
			#We install each .so in <install_dir>/lib folder
			install(
				TARGETS ${PROJECT_NAME}
				COMPONENT ${PROJECT_NAME}
				CONFIGURATIONS Release RelWithDebInfo
				EXPORT ${PROJECT_NAME}
				LIBRARY DESTINATION lib/${PROJECTS_PLATFORM}/Release
			)
			install(
				TARGETS ${PROJECT_NAME}
				COMPONENT ${PROJECT_NAME}
				CONFIGURATIONS Debug
				LIBRARY DESTINATION lib/${PROJECTS_PLATFORM}/Debug
			)
		endif()
	endif ()
else ()
	if ( NOT glslang_FOUND )
		message( SEND_ERROR "glslang library not found." )
	endif ()
	if ( NOT SPIRV_FOUND )
		message( SEND_ERROR "SPIRV library not found." )
	endif ()
endif ()
