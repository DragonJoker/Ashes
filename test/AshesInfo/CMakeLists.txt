set( FOLDER_NAME AshesInfo )
project( "${FOLDER_NAME}" )

set( BinLibraries
	${BinLibraries}
	common
	ashes
)

if ( WIN32 )
elseif ( APPLE )
else ()
	set( BinLibraries
		${BinLibraries}
		X11
	)
endif ()

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
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "Utils"
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${TARGET_INCLUDE_DIRS}
	${XCB_INCLUDE_DIRS}
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${TARGET_DEFINITIONS}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	${BinLibraries}
)
add_dependencies( ${PROJECT_NAME}
	${ENABLED_RENDERERS}
)
