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
			${SRCDIR}/${CURDIR}${SUBDIR}/*.h
			${SRCDIR}/${CURDIR}${SUBDIR}/*.hpp
			${SRCDIR}/${CURDIR}${SUBDIR}/*.inl
	)
	install(
		FILES ${_HEADERS}
		COMPONENT ${TARGET_NAME}_dev
		DESTINATION include/${TARGET_NAME}/${CURDIR}${SUBDIR}
	)
endmacro()

macro( target_install_headers TARGET_NAME HDR_FOLDER )
	list_subdirs( _SUBDIRS ${HDR_FOLDER} )
	foreach( _SUBDIR ${_SUBDIRS} )
		target_install_subdir_headers( ${TARGET_NAME} ${HDR_FOLDER} ${_SUBDIR} "" )
		list_subdirs( _SUBSUBDIRS ${HDR_FOLDER}/${_SUBDIR} )
		foreach( _SUBSUBDIR ${_SUBSUBDIRS} )
			target_install_subdir_headers( ${TARGET_NAME} ${HDR_FOLDER} ${_SUBSUBDIR} "${_SUBDIR}/" )
			list_subdirs( _SUBSUBSUBDIRS ${HDR_FOLDER}/${_SUBDIR}/${_SUBSUBDIR} )
			foreach( _SUBSUBSUBDIR ${_SUBSUBSUBDIRS} )
				target_install_subdir_headers( ${TARGET_NAME} ${HDR_FOLDER} ${_SUBSUBSUBDIR} "${_SUBDIR}/${_SUBSUBDIR}/" )
			endforeach()
		endforeach()
	endforeach()

	file(
		GLOB
			TARGET_HEADERS
			${HDR_FOLDER}/*.h
			${HDR_FOLDER}/*.hpp
			${HDR_FOLDER}/*.inl
	)
	install(
		FILES ${TARGET_HEADERS}
		COMPONENT ${TARGET_NAME}_dev
		DESTINATION include/${TARGET_NAME}
	)
endmacro()