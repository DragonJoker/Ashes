project( DesktopUtils )

file( GLOB ${PROJECT_NAME}_SRC_FILES Src/*.c Src/*.cc Src/*.cpp )
file( GLOB ${PROJECT_NAME}_HDR_FILES Src/*.h Src/*.hh Src/*.hpp Src/*.inl Src/*.inc )

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../FreeType/include
	${CMAKE_CURRENT_SOURCE_DIR}/../FreeType/include/freetype
)

add_forced_include( "${CMAKE_CURRENT_SOURCE_DIR}/Src/pch.h" )
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
	Utils
	RenderLib
	FreeType
)
