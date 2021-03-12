project( "Test-${FOLDER_NAME}" )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

file( GLOB SOURCE_FILES
	Src/*.cpp
)

file( GLOB HEADER_FILES
	Src/*.hpp
	Src/*.inl
)

add_executable( ${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
)
target_link_libraries( ${TARGET_NAME} PRIVATE
	ashes::test::Common
)
