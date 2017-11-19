project( RenderLib )

file( GLOB ${PROJECT_NAME}_SRC_FILES Src/*.c Src/*.cc Src/*.cpp )
file( GLOB ${PROJECT_NAME}_HDR_FILES Src/*.h Src/*.hh Src/*.hpp Src/*.inl Src/*.inc )
file( GLOB ${PROJECT_NAME}_DBG_FILES Src/*.natvis )

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Src
)

add_forced_include( "${CMAKE_CURRENT_SOURCE_DIR}/Src/pch.h" )
add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_DBG_FILES}
)
add_target_precompiled_header( ${PROJECT_NAME}
	pch.h
	pch.cpp
	${CMAKE_CXX_FLAGS}
	${${PROJECT_NAME}_SRC_FILES}
)
