if ( ASHES_BUILD_TESTS OR ASHES_BUILD_INFO OR ASHES_BUILD_SW_SAMPLES )
	set( TARGET_INCLUDE_DIRS
		${Ashes_SOURCE_DIR}/include
		${Ashes_BINARY_DIR}/include
		${Vulkan_INCLUDE_DIR}
	)
endif ()

if ( ASHES_BUILD_TESTS )
	find_package( wxWidgets COMPONENTS core base )
	if( APPLE )
		set( GTK2_FOUND TRUE )
		set( Ashes_BINARY_LIBRARIES
			${Ashes_BINARY_LIBRARIES}
			"-framework AppKit"
			"-framework QuartzCore"
		)
	elseif( WIN32 )
		set( GTK2_FOUND TRUE )
	else ()
		find_package( GTK2 )
	endif()

	if( wxWidgets_FOUND AND GTK2_FOUND )
		file( GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*-* )
		set( TEST_ID 0 )

		foreach ( child ${children} )
			if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child} )
				set( TARGET_NAME Test-${child} )
				add_subdirectory( ${child} )
				target_compile_features( ${TARGET_NAME} PRIVATE
					cxx_std_17
				)
				target_compile_definitions( ${TARGET_NAME} PRIVATE
					TEST_ID=${TEST_ID}
				)
				set_target_properties( ${TARGET_NAME} PROPERTIES
					CXX_STANDARD 17
					CXX_EXTENSIONS OFF
					FOLDER "${Ashes_BASE_DIR}/Tests"
				)
				math( EXPR TEST_ID "${TEST_ID} + 1" )
			endif ()
		endforeach ()
	endif ()
endif ()

if ( ASHES_BUILD_INFO )
	add_subdirectory( AshesInfo )
endif ()

if ( ASHES_BUILD_SW_SAMPLES )
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../external/glm )
	include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../external/gli )
	set( Vulkan_FOUND ON )
	set( Vulkan_LIBRARY ashes::ashes )
	add_subdirectory( Vulkan )
	set(EXAMPLES
		bloom
		computecloth
		computecullandlod
		computeheadless
		computenbody
		computeparticles
		computeraytracing
		computeshader
		conditionalrender
		conservativeraster
		debugmarker
		deferred
		deferredmultisampling
		deferredshadows
		descriptorsets
		displacement
		distancefieldfonts
		dynamicuniformbuffer
		gears
		geometryshader
		hdr
		imgui
		indirectdraw
		inlineuniformblocks
		inputattachments
		instancing
		mesh
		multisampling
		multithreading
		multiview
		negativeviewportheight
		nv_ray_tracing_basic
		nv_ray_tracing_shadows
		nv_ray_tracing_reflections
		occlusionquery
		offscreen
		parallaxmapping
		particlefire
		pbrbasic
		pbribl
		pbrtexture
		pipelines
		pipelinestatistics
		pushconstants
		pushdescriptors
		radialblur
		renderheadless
		scenerendering
		screenshot
		shadowmapping
		shadowmappingomni
		shadowmappingcascade
		skeletalanimation
		specializationconstants
		sphericalenvmapping
		ssao
		stencilbuffer
		subpasses
		terraintessellation
		tessellation
		textoverlay
		texture
		texture3d
		texturearray
		texturecubemap
		texturemipmapgen
		texturesparseresidency
		triangle
		viewportarray
		vulkanscene
	)
	foreach ( EXAMPLE ${EXAMPLES} )
		if ( TARGET ${EXAMPLE} )
			set_property( TARGET ${EXAMPLE} PROPERTY FOLDER "${Ashes_BASE_DIR}/SWExamples" )
		endif ()
	endforeach ( EXAMPLE )
	set_property( TARGET base PROPERTY FOLDER "${Ashes_BASE_DIR}/SWExamples" )
endif ()

