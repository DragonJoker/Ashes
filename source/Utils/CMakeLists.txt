project( Utils )

file( GLOB ${PROJECT_NAME}_SRC_FILES Src/*.c Src/*.cc Src/*.cpp )
file( GLOB ${PROJECT_NAME}_HDR_FILES Src/*.h Src/*.hh Src/*.hpp Src/*.inl Src/*.inc )

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
