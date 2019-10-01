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

add_executable( ${PROJECT_NAME}
	${SOURCE_FILES}
	${HEADER_FILES}
)
set_target_properties( ${TARGET_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "Utils"
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${TestIncludeDirectories}
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	TEST_ID=${TEST_ID}
	_FILE_OFFSET_BITS=64
	_LARGE_FILES
	_UNICODE
)

if ( NOT WIN32 )
	target_link_libraries( ${PROJECT_NAME} PRIVATE
		${BinLibraries}
		X11
		dl
	)
	target_compile_definitions( ${PROJECT_NAME} PRIVATE
		VK_USE_PLATFORM_XLIB_KHR=1
	)
else ()
	target_link_libraries( ${PROJECT_NAME} PRIVATE
		${BinLibraries}
	)
endif ()

add_dependencies( ${PROJECT_NAME}
	${ENABLED_RENDERERS}
)
