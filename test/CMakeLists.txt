find_package( wxWidgets COMPONENTS core base )
if( NOT WIN32 )
	find_package( GTK2 )
else()
	set( GTK2_FOUND TRUE )
endif()

if( wxWidgets_FOUND AND GTK2_FOUND )
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )

	set( TestIncludeDirectories
		${CMAKE_SOURCE_DIR}/util/Src
		${CMAKE_BINARY_DIR}/util/Src
		${CMAKE_SOURCE_DIR}/include
		${CMAKE_BINARY_DIR}/include
		${CMAKE_SOURCE_DIR}/external/gli
		${CMAKE_SOURCE_DIR}/external/glm
		${CMAKE_SOURCE_DIR}/external/imgui
		${wxWidgets_INCLUDE_DIRS}
		${GTK2_INCLUDE_DIRS}
	)

	set( BinLibraries
		${BinLibraries}
		common
		ashes
		ashespp
		util
	)

	file( GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*-* )
	set( TEST_ID 0 )

	foreach ( child ${children} )
		if ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child} )
			add_subdirectory( ${child} )
			target_compile_definitions( Test-${child} PRIVATE
				TEST_ID=${TEST_ID}
				_FILE_OFFSET_BITS=64
				_LARGE_FILES
				_UNICODE
			)

			if( NOT WIN32 )
				target_compile_definitions( Test-${child} PRIVATE
					__WXGTK__
				)
			endif()

			if( IsWxDebug )
				target_compile_definitions( Test-${child} PRIVATE
					__WXDEBUG__
				)
			endif()

			math( EXPR TEST_ID "${TEST_ID} + 1" )
		endif ()
	endforeach ()

	add_subdirectory( AshesInfo )

	if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/CMakeLists.txt )
		include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../external/glm )
		include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../external/gli )
		set( Vulkan_FOUND ON )
		set( Vulkan_INCLUDE_DIR ${Vulkan_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include )
		set( Vulkan_LIBRARY ashes )
		add_subdirectory( Vulkan )
		set(EXAMPLES
			bloom
			computecloth
			computecullandlod
			computeheadless
			computenbody
			computeparticles
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
			raytracing
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
			set_property( TARGET ${EXAMPLE} PROPERTY FOLDER "Examples" )
		endforeach ( EXAMPLE )
		set_property( TARGET base PROPERTY FOLDER "Examples" )
	endif ()
endif ()
