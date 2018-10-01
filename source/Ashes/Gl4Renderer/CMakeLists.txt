project( Gl4Renderer )

find_package( OpenGL )

if ( OpenGL_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${SPIRV_CROSS_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/../Ashes/Src
		${CMAKE_CURRENT_SOURCE_DIR}/../Ashes/Src
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

	set( ${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES
		${SPIRV_CROSS_DIR}/spirv_cross.cpp
		${SPIRV_CROSS_DIR}/spirv_cfg.cpp
		${SPIRV_CROSS_DIR}/spirv_glsl.cpp
	)

	set( ${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES
		${SPIRV_CROSS_DIR}/GLSL.std.450.h
		${SPIRV_CROSS_DIR}/spirv_common.hpp
		${SPIRV_CROSS_DIR}/spirv.hpp
		${SPIRV_CROSS_DIR}/spirv_cross.hpp
		${SPIRV_CROSS_DIR}/spirv_cfg.hpp
		${SPIRV_CROSS_DIR}/spirv_glsl.hpp
	)

	if ( ASHES_STATIC_RENDERERS )
		add_definitions(
			-DGl4Renderer_STATIC
		)
		add_library( ${PROJECT_NAME} STATIC
			${${PROJECT_NAME}_CXX_FILES}
			${${PROJECT_NAME}_HDR_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
		)
	else ()
		add_definitions(
			-DGl4Renderer_EXPORTS
		)
		add_library( ${PROJECT_NAME} SHARED
			${${PROJECT_NAME}_CXX_FILES}
			${${PROJECT_NAME}_HDR_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
			${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
		)
	endif ()

	if ( BUILD_ASHES )
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
		Ashes
	)

	add_dependencies( ${PROJECT_NAME}
		Ashes
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

	source_group( "Header Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES} )
	source_group( "Source Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES} )

	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Ashes" )

	set( ENABLED_RENDERERS
		${ENABLED_RENDERERS}
		Gl4Renderer
		PARENT_SCOPE
	)

	if ( BUILD_ASHES )
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
	endif ()
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
