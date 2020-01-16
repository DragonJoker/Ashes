set( FOLDER_NAME AshesInfo )
project( "${FOLDER_NAME}" )

set( Ashes_BINARY_LIBRARIES
	${Ashes_BINARY_LIBRARIES}
	ashes::common
	ashes::ashes
)

if ( WIN32 )
elseif ( APPLE )
else ()
	set( Ashes_BINARY_LIBRARIES
		${Ashes_BINARY_LIBRARIES}
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
	FOLDER "${Ashes_BASE_DIR}/Utils"
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${TARGET_INCLUDE_DIRS}
	${XCB_INCLUDE_DIRS}
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${Ashes_BINARY_DEFINITIONS}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	${Ashes_BINARY_LIBRARIES}
)
add_dependencies( ${PROJECT_NAME}
	${ENABLED_RENDERERS}
)
