project( Renderer )

find_package( glslang )
find_package( SPIRV )

if ( glslang_FOUND AND SPIRV_FOUND )
	include_directories(
		${CMAKE_SOURCE_DIR}
		${glslang_INCLUDE_DIR}
		${SPIRV_INCLUDE_DIR}
	)

	set( BinaryLibraries
		${BinaryLibraries}
		${glslang_LIBRARIES}
		${SPIRV_LIBRARIES}
		PARENT_SCOPE
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
	)

	add_target_precompiled_header( ${PROJECT_NAME}
		pch.h
		pch.cpp
		${CMAKE_CXX_FLAGS}
		${${PROJECT_NAME}_SRC_FILES}
	)

	target_link_libraries( ${PROJECT_NAME}
		${VkLib_LIBRARIES}
	)

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Render" )
else ()
	if ( NOT glslang_FOUND )
		message( SEND_ERROR "glslang library not found." )
	endif ()
	if ( NOT SPIRV_FOUND )
		message( SEND_ERROR "SPIRV library not found." )
	endif ()
endif ()