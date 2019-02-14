macro( list_subdirs RESULT CURDIR )
	file( GLOB _CHILDREN RELATIVE ${CURDIR} ${CURDIR}/* )
	set( _SUBFOLDERS "" )

	foreach( _CHILD ${_CHILDREN} )
		if ( IS_DIRECTORY ${CURDIR}/${_CHILD} )
			list( APPEND _SUBFOLDERS ${_CHILD} )
		endif ()
	endforeach()
	set( ${RESULT} ${_SUBFOLDERS} )
endmacro()

macro( target_install_subdir_headers TARGET_NAME SRCDIR SUBDIR CURDIR )
	file(
		GLOB
			_HEADERS
			${SRCDIR}${CURDIR}${SUBDIR}/*.h
			${SRCDIR}${CURDIR}${SUBDIR}/*.hpp
			${SRCDIR}${CURDIR}${SUBDIR}/*.inl
	)
	install(
		FILES ${_HEADERS}
		COMPONENT ${TARGET_NAME}_dev
		DESTINATION include/${TARGET_NAME}/${CURDIR}${SUBDIR}
	)
endmacro()

macro( target_install_headers TARGET_NAME SRCDIR )
	if ( "${SRCDIR}" STREQUAL "" )
		set( _SRCDIR ${SRCDIR} )
	else ()
		set( _SRCDIR ${SRCDIR}/ )
	endif ()
	list_subdirs( _SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/${_SRCDIR} )
	foreach( _SUBDIR ${_SUBDIRS} )
		target_install_subdir_headers( ${TARGET_NAME} ${_SRCDIR} ${_SUBDIR} "" )
		list_subdirs( _SUBSUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/${_SRCDIR}${_SUBDIR} )
		foreach( _SUBSUBDIR ${_SUBSUBDIRS} )
			target_install_subdir_headers( ${TARGET_NAME} ${_SRCDIR} ${_SUBSUBDIR} "${_SUBDIR}/" )
			list_subdirs( _SUBSUBSUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/${_SRCDIR}${_SUBDIR}/${_SUBSUBDIR} )
			foreach( _SUBSUBSUBDIR ${_SUBSUBSUBDIRS} )
				target_install_subdir_headers( ${TARGET_NAME} ${_SRCDIR} ${_SUBSUBSUBDIR} "${_SUBDIR}/${_SUBSUBDIR}/" )
			endforeach()
		endforeach()
	endforeach()

	file(
		GLOB
			TARGET_HEADERS
			${CMAKE_CURRENT_SOURCE_DIR}/${_SRCDIR}*.h
			${CMAKE_CURRENT_SOURCE_DIR}/${_SRCDIR}*.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/${_SRCDIR}*.inl
			${CMAKE_CURRENT_BINARY_DIR}/${_SRCDIR}*.h
			${CMAKE_CURRENT_BINARY_DIR}/${_SRCDIR}*.hpp
			${CMAKE_CURRENT_BINARY_DIR}/${_SRCDIR}*.inl
	)
	install(
		FILES ${TARGET_HEADERS}
		COMPONENT ${TARGET_NAME}_dev
		DESTINATION include/${TARGET_NAME}
	)
endmacro()