set( FOLDER_NAME 02-SwapChainCreation )
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

include_directories( ${CMAKE_SOURCE_DIR}/Test/00-Common/Src )

add_executable( ${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
)
target_link_libraries( ${PROJECT_NAME}
	Test-00-Common
	${BinLibraries}
)
