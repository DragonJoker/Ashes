project( RenderLib )

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

file( GLOB ${PROJECT_NAME}_DBG_FILES
	*.natvis
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/Renderer
)

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_DBG_FILES}
)

add_target_precompiled_header( ${PROJECT_NAME}
	RenderLibPch.hpp
	RenderLibPch.cpp
	${CMAKE_CXX_FLAGS}
	${${PROJECT_NAME}_SRC_FILES}
)

add_dependencies( ${PROJECT_NAME}
	Renderer
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Render" )
