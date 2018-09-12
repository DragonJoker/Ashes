set( FOLDER_NAME RendererInfo )
project( "${FOLDER_NAME}" )

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

add_executable( ${PROJECT_NAME}
	${SOURCE_FILES}
	${HEADER_FILES}
)

target_link_libraries( ${PROJECT_NAME}
	${VkLib_LIBRARIES}
	Utils
	Renderer
	${BinLibraries}
)

if ( NOT WIN32 )
	target_link_libraries( ${PROJECT_NAME}
		X11
	)
endif ()

add_dependencies( ${PROJECT_NAME}
	VkRenderer
	GlRenderer
	Gl3Renderer
	D3D11Renderer
	TestRenderer
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Utils" )

