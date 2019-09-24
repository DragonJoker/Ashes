set( FOLDER_NAME AshesInfo )
project( "${FOLDER_NAME}" )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

file( GLOB SOURCE_FILES
	Src/*.c
	Src/*.cc
	Src/*.cpp
)

file( GLOB HEADER_FILES
	Src/*.hpp
	Src/*.inl
)

include_directories( ${TestIncludeDirectories} )

add_executable( ${PROJECT_NAME}
	${SOURCE_FILES}
	${HEADER_FILES}
)
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Utils" )

target_compile_definitions( ${PROJECT_NAME} PRIVATE
	TEST_ID=${TEST_ID}
	_FILE_OFFSET_BITS=64
	_LARGE_FILES
	_UNICODE
)

target_link_libraries( ${PROJECT_NAME}
	${BinLibraries}
)

if ( NOT WIN32 )
	target_link_libraries( ${PROJECT_NAME}
		X11
		dl
	)
	target_compile_definitions( ${PROJECT_NAME} PRIVATE
		VK_USE_PLATFORM_XLIB_KHR=1
	)
endif ()

add_dependencies( ${PROJECT_NAME}
	${ENABLED_RENDERERS}
)
