project( GlRendererCommon )

if ( ${CMAKE_VERSION} VERSION_GREATER "3.10.2" ) 
	cmake_policy( SET CMP0072 NEW )
endif()

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

find_package( OpenGL )

if ( OpenGL_FOUND )
	if ( WIN32 )
		set( BinLibraries
			${BinLibraries}
			OpenGL::GL
		)
	else ()
		find_package( X11 )
		if ( ASHES_USE_XLIB_EGL )
			add_definitions( -DASHES_USE_XLIB_EGL )
		endif ()
		find_package( XCB REQUIRED COMPONENTS xcb )
		set( BinLibraries
			${BinLibraries}
			${XCB_LIBRARIES}
			${X11_X11_LIB}
			${X11_Xrandr_LIB}
			X11-xcb
			OpenGL::EGL
			OpenGL::GLX
			OpenGL::OpenGL

		)
	endif ()

	file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.c
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cc
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.cpp
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_NVS_FILES
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.natvis
	)

	file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.h
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hh
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.hpp
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inl
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}/*.inc
	)

	add_library( ${PROJECT_NAME} STATIC
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_HDR_FILES}
	)
	add_library( ashes::${PROJECT_NAME}
		ALIAS
		${PROJECT_NAME}
	)
	target_include_directories( ${PROJECT_NAME} PRIVATE
		${TARGET_INCLUDE_DIRS}
		${CMAKE_CURRENT_BINARY_DIR}
		${ASHES_SOURCE_DIR}/source/ashes/renderer/${PROJECT_NAME}
	)
	target_link_libraries( ${PROJECT_NAME} PRIVATE
		${BinLibraries}
		${RendererDependencies}
	)
	target_compile_features( ${PROJECT_NAME} PRIVATE
		cxx_std_17
	)
	target_compile_definitions( ${PROJECT_NAME} PRIVATE
		${TARGET_CXX_DEFINITIONS}
	)
	target_compile_options( ${PROJECT_NAME} PRIVATE
		${TARGET_CXX_OPTIONS}
	)
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "Ashes/Renderer"
	)

	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Platform "Platform" )
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
