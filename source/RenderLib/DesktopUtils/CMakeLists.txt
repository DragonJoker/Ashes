project( DesktopUtils )

file( GLOB ${PROJECT_NAME}_SRC_FILES
	*.c
	*.cc
	*.cpp
)

file( GLOB ${PROJECT_NAME}_HDR_FILES
	*.h
	*.hh
	*.hpp
	*.inl
	*.inc
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Core/FreeType/include
	${CMAKE_SOURCE_DIR}/Core/FreeType/include/freetype
)

add_forced_include( "${CMAKE_CURRENT_SOURCE_DIR}/pch.h" )

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)

add_target_precompiled_header( ${PROJECT_NAME}
	pch.h
	pch.cpp
	${CMAKE_CXX_FLAGS}
	${${PROJECT_NAME}_SRC_FILES}
)

add_dependencies( ${PROJECT_NAME}
	GlRenderer
	VkRenderer
	RenderLib
	FreeType
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Main" )
