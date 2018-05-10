project( Gl3Renderer )

find_package( OpenGL )

if ( OpenGL_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/../Renderer/Src
		${CMAKE_CURRENT_SOURCE_DIR}/../Renderer/Src
		${CMAKE_CURRENT_SOURCE_DIR}/Src
		${CMAKE_CURRENT_BINARY_DIR}/Src
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_CXX_FILES
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

	if ( RENDERER_STATIC_RENDERERS )
		add_definitions(
			-DGl3Renderer_STATIC
		)
		add_library( ${PROJECT_NAME} STATIC
			${${PROJECT_NAME}_CXX_FILES}
			${${PROJECT_NAME}_HDR_FILES}
		)
	else ()
		add_definitions(
			-DGl3Renderer_EXPORTS
		)
		add_library( ${PROJECT_NAME} SHARED
			${${PROJECT_NAME}_CXX_FILES}
			${${PROJECT_NAME}_HDR_FILES}
		)
	endif ()

	if ( BUILD_RENDERERLIB )
		add_target_precompiled_header( ${PROJECT_NAME}
			Src/GlRendererPch.hpp
			Src/GlRendererPch.cpp
			${CMAKE_CXX_FLAGS}
			${${PROJECT_NAME}_CXX_FILES}
		)
	else ()
		add_target_precompiled_header( ${PROJECT_NAME}
			GlRendererPch.hpp
			GlRendererPch.cpp
			${CMAKE_CXX_FLAGS}
			${${PROJECT_NAME}_CXX_FILES}
		)
	endif ()

	target_link_libraries( ${PROJECT_NAME}
		${OPENGL_gl_LIBRARY}
		${BinLibraries}
		Renderer
	)

	add_dependencies( ${PROJECT_NAME}
		Renderer
	)

	parse_subdir_files( Src/Buffer "Buffer" )
	parse_subdir_files( Src/Command "Command" )
	parse_subdir_files( Src/Command/Commands "Command\\\\Commands" )
	parse_subdir_files( Src/Core "Core" )
	parse_subdir_files( Src/Descriptor "Descriptor" )
	parse_subdir_files( Src/Enum "Enum" )
	parse_subdir_files( Src/Image "Image" )
	parse_subdir_files( Src/Miscellaneous "Miscellaneous" )
	parse_subdir_files( Src/Pipeline "Pipeline" )
	parse_subdir_files( Src/RenderPass "RenderPass" )
	parse_subdir_files( Src/Shader "Shader" )
	parse_subdir_files( Src/Sync "Sync" )
	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Renderer" )

	if ( RENDERER_STATIC_RENDERERS )
		if ( WIN32 )
			#We install each .lib in <install_dir>/lib folder
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
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
