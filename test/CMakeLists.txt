find_package( wxWidgets COMPONENTS core base )
if( NOT WIN32 )
	find_package( GTK2 )
else()
	set( GTK2_FOUND TRUE )
endif()

if( wxWidgets_FOUND AND GTK2_FOUND )
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )

	set( TestAdditionalDefinitions
		"-D_FILE_OFFSET_BITS=64"
		"-D_LARGE_FILES"
		"-D_UNICODE"
	)

	if( NOT WIN32 )
		set( TestAdditionalDefinitions
			${TestAdditionalDefinitions}
			"-D__WXGTK__"
		)
	endif()

	if( IsWxDebug )
		set( TestAdditionalDefinitions
			${TestAdditionalDefinitions}
			"-D__WXDEBUG__"
		)
	endif()

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

	add_subdirectory( 00-Common )
	add_subdirectory( 01-DeviceCreation )
	add_subdirectory( 02-SwapChainCreation )
	add_subdirectory( 03-VertexLayout )
	add_subdirectory( 04-StagingBuffer )
	add_subdirectory( 05-Texture2D )
	add_subdirectory( 06-UniformBuffers )
	add_subdirectory( 07-Texture1D )
	add_subdirectory( 08-TextureBuffer )
	add_subdirectory( 09-FrameBuffer )
	add_subdirectory( 10-SpinningCube )
	add_subdirectory( 11-Texture3D )
	add_subdirectory( 12-TextureCube )
	add_subdirectory( 13-PushConstants )
	add_subdirectory( 14-RenderToCube )
	add_subdirectory( 15-Instantiation )
	add_subdirectory( 16-ComputePipeline )
	add_subdirectory( 17-CompressedTexture2D )
	add_subdirectory( 18-Texture2DMipmaps )
	add_subdirectory( 19-CompressedTexture2DArray )
	add_subdirectory( 20-DynamicUniformBuffer )
	add_subdirectory( 21-SpecialisationConstants )
	add_subdirectory( 22-SPIRVSpecialisationConstants )
	add_subdirectory( 23-Bloom )
	add_subdirectory( AshesInfo )
	
	if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/CMakeLists.txt )
		set( Vulkan_FOUND ON )
		set( Vulkan_INCLUDE_DIR ${Vulkan_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include )
		set( Vulkan_LIBRARY ashes ${ENABLED_RENDERERS} )
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
