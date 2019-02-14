find_package( Doxygen )
option( PROJECTS_GENERATE_DOC "Generate Doxygen documentation" FALSE )

#--------------------------------------------------------------------------------------------------
#	Function :	target_add_doc
#	Generates doc for given target
#--------------------------------------------------------------------------------------------------
function( target_add_doc TARGET_NAME LANGUAGE EXT_LIST )
	find_package( HTMLHelp )
	set( TARGET_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR} )
	set( TARGET_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR} )
	set( TARGET_VERSION_BUILD ${${PROJECT_NAME}_VERSION_BUILD} )
	if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Doc/${TARGET_NAME}Doxygen.css )
		set( TARGET_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/Doc/${TARGET_NAME}Doxygen.css )
	endif ()
	if ( "${LANGUAGE}" STREQUAL "" )
		set( CHM_NAME ${TARGET_NAME}.chm )
		if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Doc/${TARGET_NAME}.Doxyfile )
			file( COPY
				${CMAKE_CURRENT_SOURCE_DIR}/Doc/${TARGET_NAME}.Doxyfile
				DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.Doxyfile
			)
		elseif ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Doc/${TARGET_NAME}.Doxyfile.in )
			configure_file(
				${CMAKE_CURRENT_SOURCE_DIR}/Doc/${TARGET_NAME}.Doxyfile.in
				${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.Doxyfile
				@ONLY
				NEWLINE_STYLE LF
			)
		elseif ( EXISTS ${CMAKE_TEMPLATES_DIR}/Doxyfile.in )
			configure_file(
				${CMAKE_TEMPLATES_DIR}/Doxyfile.in
				${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.Doxyfile
				@ONLY
				NEWLINE_STYLE LF
			)
		else ()
			message( SEND_ERROR "Couldn't find a doxyfile or a template doxyfile" )
		endif ()
		if ( DOXYGEN_FOUND AND PROJECTS_GENERATE_DOC )
			set( DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.Doxyfile )
			set( DOXYGEN_OUTPUT ${PROJECTS_DOCUMENTATION_OUTPUT_DIR}/${TARGET_NAME} )
			file( MAKE_DIRECTORY ${DOXYGEN_OUTPUT} )
			set( DOXYGEN_TARGET_NAME ${TARGET_NAME}_Doc )
			add_custom_target(
				${DOXYGEN_TARGET_NAME}
				COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
				COMMENT "Building Doxygen documentation for ${TARGET_NAME}"
				VERBATIM
			)
			set_property( TARGET ${DOXYGEN_TARGET_NAME} PROPERTY FOLDER "Documentation/${TARGET_NAME}" )
			set( _DOC_FILE ${PROJECTS_DOCUMENTATION_OUTPUT_DIR}/${TARGET_NAME}/${CHM_NAME} )
			if ( EXISTS ${_DOC_FILE} )
				install(
					FILES ${_DOC_FILE}
					DESTINATION share/doc/${TARGET_NAME}
					COMPONENT ${DOXYGEN_TARGET_NAME}
				)
			endif ()
		endif ()
	else ()
		set( CHM_NAME ${TARGET_NAME}-${LANGUAGE}.chm )
		configure_file(
			${CMAKE_CURRENT_SOURCE_DIR}/Doc/${TARGET_NAME}.Doxyfile.in
			${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_${LANGUAGE}.Doxyfile
			@ONLY
			NEWLINE_STYLE LF
		)
		if ( DOXYGEN_FOUND AND PROJECTS_GENERATE_DOC )
			set( DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_${LANGUAGE}.Doxyfile )
			set( DOXYGEN_OUTPUT ${PROJECTS_DOCUMENTATION_OUTPUT_DIR}/${TARGET_NAME}/${LANGUAGE} )
			file( MAKE_DIRECTORY ${DOXYGEN_OUTPUT} )
			set( DOXYGEN_TARGET_NAME ${TARGET_NAME}_${LANGUAGE}_Doc )
			add_custom_target(
				${DOXYGEN_TARGET_NAME}
				COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
				COMMENT "Building Doxygen ${LANGUAGE} documentation for ${TARGET_NAME}"
				VERBATIM
			)
			set_property( TARGET ${DOXYGEN_TARGET_NAME} PROPERTY FOLDER "Documentation/${TARGET_NAME}" )
			set( _DOC_FILE ${PROJECTS_DOCUMENTATION_OUTPUT_DIR}/${TARGET_NAME}/${LANGUAGE}/${CHM_NAME} )
			if ( EXISTS ${_DOC_FILE} )
				install(
					FILES ${_DOC_FILE}
					DESTINATION share/doc/${LANGUAGE}/${TARGET_NAME}
					COMPONENT ${DOXYGEN_TARGET_NAME}
				)
			endif ()
		endif ()
	endif ()
endfunction( target_add_doc )

#--------------------------------------------------------------------------------------------------
#	Macro :	add_doc
#	Generates doc for current project
#--------------------------------------------------------------------------------------------------
macro( add_doc LANGUAGE EXT_LIST )
	target_add_doc( ${PROJECT_NAME} "${LANGUAGE}" "${EXT_LIST}" )
endmacro( add_doc )
