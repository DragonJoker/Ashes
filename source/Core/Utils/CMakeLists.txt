project( Utils )

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

add_library( ${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )
