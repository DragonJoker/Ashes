project( Utils )

include_directories(
	${CMAKE_SOURCE_DIR}/Renderer/Renderer/Src
	${CMAKE_BINARY_DIR}/Renderer/Renderer/Src
)

file( GLOB ${PROJECT_NAME}_SRC_FILES
	Src/*.c
	Src/*.cc
	Src/*.cpp
)

file( GLOB ${PROJECT_NAME}_HDR_FILES
	Src/*.h
	Src/*.hh
	Src/*.hpp
	Src/*.inl
	Src/*.inc
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )

#We copy each lib in <install_dir>/lib folder
install(
	TARGETS ${PROJECT_NAME}
	COMPONENT ${PROJECT_NAME}_dev
	CONFIGURATIONS Release RelWithDebInfo
	EXPORT ${PROJECT_NAME}
	ARCHIVE DESTINATION lib/${PROJECTS_PLATFORM}/Release
)
install(
	TARGETS ${PROJECT_NAME}
	COMPONENT ${PROJECT_NAME}_dev
	CONFIGURATIONS Debug
	ARCHIVE DESTINATION lib/${PROJECTS_PLATFORM}/Debug
)

#For libs, we install headers to <install_dir>/include/${PROJECT_NAME}
target_install_headers( ${PROJECT_NAME} Src )