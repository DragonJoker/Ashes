project( Renderer )

option( RENDERLIB_GLSL_TO_SPV "Enable Comping from GLSL to SPIR-V, using glslang and SPIRV libraries" TRUE )

if ( RENDERLIB_GLSL_TO_SPV )
	set( RENDERLIB_GLSL_TO_SPV 1 )
	find_package( glslang )
	find_package( SPIRV )

	if ( glslang_FOUND AND SPIRV_FOUND )
		include_directories(
			${glslang_INCLUDE_DIR}
			${SPIRV_INCLUDE_DIR}
		)
		set( BinaryLibraries
			${BinaryLibraries}
			${glslang_LIBRARIES}
			${SPIRV_LIBRARIES}
			PARENT_SCOPE
		)
	endif ()
else ()
	set( RENDERLIB_GLSL_TO_SPV 0 )
	set( SPIRV_FOUND TRUE )
	set( glslang_FOUND TRUE )
endif ()

set( ${PROJECT_NAME}_CONFIG_HEADER
	${CMAKE_CURRENT_BINARY_DIR}/Config.hpp
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp.in
	${${PROJECT_NAME}_CONFIG_HEADER}
	NEWLINE_STYLE LF
)

if ( glslang_FOUND AND SPIRV_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
	)

	file( GLOB ${PROJECT_NAME}_SRC_FILES
		*.c
		*.cc
		*.cpp
	)

	file( GLOB ${PROJECT_NAME}_HDR_FILES
		*.h
		*.hh
		*.hpp
		*.inl
		*.inc
	)

	add_library( ${PROJECT_NAME} STATIC
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_CONFIG_HEADER}
	)

	add_target_precompiled_header( ${PROJECT_NAME}
		RendererPch.hpp
		RendererPch.cpp
		${CMAKE_CXX_FLAGS}
		${${PROJECT_NAME}_SRC_FILES}
	)

	target_link_libraries( ${PROJECT_NAME}
		${VkLib_LIBRARIES}
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Render" )
else ()
	if ( NOT glslang_FOUND )
		message( SEND_ERROR "glslang library not found." )
	endif ()
	if ( NOT SPIRV_FOUND )
		message( SEND_ERROR "SPIRV library not found." )
	endif ()
endif ()
