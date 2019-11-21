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
		find_package( X11 COMPONENTS REQUIRED X11 Xrandr )
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
	add_dependencies( ${PROJECT_NAME}
		${RendererDependencies}
	)

	parse_subdir_files_ex( ${ASHES_SOURCE_DIR} ashes/renderer Platform "Platform" )

	if ( WIN32 )
		#We install each .lib in <install_dir>/lib folder
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}
			CONFIGURATIONS Release
			EXPORT ${PROJECT_NAME}
			LIBRARY DESTINATION lib
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS RelWithDebInfo
			EXPORT ${PROJECT_NAME}
			LIBRARY DESTINATION lib/RelWithDebInfo
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS Debug
			EXPORT ${PROJECT_NAME}
			LIBRARY DESTINATION lib/Debug
		)
	else ()
		#We install each .a in <install_dir>/lib folder
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}
			CONFIGURATIONS Release
			EXPORT ${PROJECT_NAME}
			ARCHIVE DESTINATION lib
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS RelWithDebInfo
			EXPORT ${PROJECT_NAME}
			ARCHIVE DESTINATION lib/RelWithDebInfo
		)
		install(
			TARGETS ${PROJECT_NAME}
			COMPONENT ${PROJECT_NAME}_dev
			CONFIGURATIONS Debug
			ARCHIVE DESTINATION lib/Debug
		)
	endif()
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
