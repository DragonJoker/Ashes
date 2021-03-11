set( _PROJECT_NAME GlRenderer )
project( ashes${_PROJECT_NAME} )

if ( ${CMAKE_VERSION} VERSION_GREATER "3.10.2" ) 
	cmake_policy( SET CMP0072 NEW )
endif()

set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

option( ASHES_GL_LOG_CALLS "Log OpenGL calls in CallLogGL.log file." OFF )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}" )

find_package( OpenGL )

if ( OpenGL_FOUND )
	if ( WIN32 )
		set( Ashes_BINARY_LIBRARIES
			${Ashes_BINARY_LIBRARIES}
			OpenGL::GL
		)
	elseif ( APPLE )
		set( Ashes_BINARY_LIBRARIES
			${Ashes_BINARY_LIBRARIES}
			"-framework OpenGL"
			"-framework AppKit"
			OpenGL::GL
		)
	else ()
		find_package( X11 )
		if ( ASHES_USE_XLIB_EGL )
			set( TARGET_CXX_DEFINITIONS
				${TARGET_CXX_DEFINITIONS}
				ASHES_USE_XLIB_EGL
			)
		endif ()
		find_package( XCB REQUIRED COMPONENTS xcb )
		set( Ashes_BINARY_LIBRARIES
			${Ashes_BINARY_LIBRARIES}
			${XCB_LIBRARIES}
			${X11_X11_LIB}
			${X11_Xrandr_LIB}
			X11-xcb
			OpenGL::EGL
			OpenGL::GLX
			OpenGL::OpenGL
		)
	endif ()
	if ( ASHES_GL_LOG_CALLS )
		set( TARGET_CXX_DEFINITIONS
			${TARGET_CXX_DEFINITIONS}
			AshesGL_LogCalls=1
		)
	else ()
		set( TARGET_CXX_DEFINITIONS
			${TARGET_CXX_DEFINITIONS}
			AshesGL_LogCalls=0
		)
	endif ()

	set( ${PROJECT_NAME}_SRC_FILES
		ash_opengl.cpp
		GlRendererPch.cpp
		GlRendererPrerequisites.cpp
	)
	set( ${PROJECT_NAME}_HDR_FILES
		ashesgl_api.hpp
		GlRendererPch.hpp
		GlRendererPrerequisites.hpp
	)
	set( ${PROJECT_NAME}_NVS_FILES
		GlRenderer.natvis
	)
	source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
	source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )
	source_group( "Natvis Files" FILES ${${PROJECT_NAME}_NVS_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Buffer/GlBuffer.cpp
		Buffer/GlBufferView.cpp
		Buffer/GlGeometryBuffers.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Buffer/GlBuffer.hpp
		Buffer/GlBufferView.hpp
		Buffer/GlGeometryBuffers.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Buffer" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Buffer" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Command/GlCommandBuffer.cpp
		Command/GlCommandPool.cpp
		Command/GlQueue.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Command/GlCommandBuffer.hpp
		Command/GlCommandPool.hpp
		Command/GlQueue.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Command" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Command" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Command/Commands/GlCommandBase.cpp
		Command/Commands/GlBeginQueryCommand.cpp
		Command/Commands/GlBeginRenderPassCommand.cpp
		Command/Commands/GlBeginSubpassCommand.cpp
		Command/Commands/GlBindComputePipelineCommand.cpp
		Command/Commands/GlBindDescriptorSetCommand.cpp
		Command/Commands/GlBindGeometryBuffersCommand.cpp
		Command/Commands/GlBindPipelineCommand.cpp
		Command/Commands/GlBlitImageCommand.cpp
		Command/Commands/GlClearAttachmentsCommand.cpp
		Command/Commands/GlClearColourCommand.cpp
		Command/Commands/GlClearColourFboCommand.cpp
		Command/Commands/GlClearDepthStencilCommand.cpp
		Command/Commands/GlClearDepthStencilFboCommand.cpp
		Command/Commands/GlCopyBufferCommand.cpp
		Command/Commands/GlCopyBufferToImageCommand.cpp
		Command/Commands/GlCopyImageCommand.cpp
		Command/Commands/GlCopyImageToBufferCommand.cpp
		Command/Commands/GlDispatchCommand.cpp
		Command/Commands/GlDispatchIndirectCommand.cpp
		Command/Commands/GlDrawCommand.cpp
		Command/Commands/GlDrawIndexedCommand.cpp
		Command/Commands/GlDrawIndexedIndirectCommand.cpp
		Command/Commands/GlDrawIndirectCommand.cpp
		Command/Commands/GlEndQueryCommand.cpp
		Command/Commands/GlEndRenderPassCommand.cpp
		Command/Commands/GlEndSubpassCommand.cpp
		Command/Commands/GlGenerateMipmapsCommand.cpp
		Command/Commands/GlMemoryBarrierCommand.cpp
		Command/Commands/GlPushConstantsCommand.cpp
		Command/Commands/GlResetEventCommand.cpp
		Command/Commands/GlResetQueryPoolCommand.cpp
		Command/Commands/GlScissorCommand.cpp
		Command/Commands/GlSetDepthBiasCommand.cpp
		Command/Commands/GlSetEventCommand.cpp
		Command/Commands/GlSetLineWidthCommand.cpp
		Command/Commands/GlViewportCommand.cpp
		Command/Commands/GlWaitEventsCommand.cpp
		Command/Commands/GlWriteTimestampCommand.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Command/Commands/GlCommandBase.hpp
		Command/Commands/GlBeginQueryCommand.hpp
		Command/Commands/GlBeginRenderPassCommand.hpp
		Command/Commands/GlBeginSubpassCommand.hpp
		Command/Commands/GlBindComputePipelineCommand.hpp
		Command/Commands/GlBindDescriptorSetCommand.hpp
		Command/Commands/GlBindGeometryBuffersCommand.hpp
		Command/Commands/GlBindPipelineCommand.hpp
		Command/Commands/GlBlitImageCommand.hpp
		Command/Commands/GlClearAttachmentsCommand.hpp
		Command/Commands/GlClearColourCommand.hpp
		Command/Commands/GlClearColourFboCommand.hpp
		Command/Commands/GlClearDepthStencilCommand.hpp
		Command/Commands/GlClearDepthStencilFboCommand.hpp
		Command/Commands/GlCopyBufferCommand.hpp
		Command/Commands/GlCopyBufferToImageCommand.hpp
		Command/Commands/GlCopyImageCommand.hpp
		Command/Commands/GlCopyImageToBufferCommand.hpp
		Command/Commands/GlDispatchCommand.hpp
		Command/Commands/GlDispatchIndirectCommand.hpp
		Command/Commands/GlDrawCommand.hpp
		Command/Commands/GlDrawIndexedCommand.hpp
		Command/Commands/GlDrawIndexedIndirectCommand.hpp
		Command/Commands/GlDrawIndirectCommand.hpp
		Command/Commands/GlEndQueryCommand.hpp
		Command/Commands/GlEndRenderPassCommand.hpp
		Command/Commands/GlEndSubpassCommand.hpp
		Command/Commands/GlGenerateMipmapsCommand.hpp
		Command/Commands/GlMemoryBarrierCommand.hpp
		Command/Commands/GlPushConstantsCommand.hpp
		Command/Commands/GlResetEventCommand.hpp
		Command/Commands/GlResetQueryPoolCommand.hpp
		Command/Commands/GlScissorCommand.hpp
		Command/Commands/GlSetDepthBiasCommand.hpp
		Command/Commands/GlSetEventCommand.hpp
		Command/Commands/GlSetLineWidthCommand.hpp
		Command/Commands/GlViewportCommand.hpp
		Command/Commands/GlWaitEventsCommand.hpp
		Command/Commands/GlWriteTimestampCommand.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Command\\Commands" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Command\\Commands" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Core/GlAutoIdIcdObject.cpp
		Core/GlContext.cpp
		Core/GlContextImpl.cpp
		Core/GlContextLock.cpp
		Core/GlContextState.cpp
		Core/GlContextStateStack.cpp
		Core/GlDebugReportCallback.cpp
		Core/GlDevice.cpp
		Core/GlDisplay.cpp
		Core/GlDisplayMode.cpp
		Core/GlIcdObject.cpp
		Core/GlInstance.cpp
		Core/GlPhysicalDevice.cpp
		Core/GlSurface.cpp
		Core/GlSwapChain.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Core/GlAutoIdIcdObject.hpp
		Core/GlContext.hpp
		Core/GlContextImpl.hpp
		Core/GlContextLock.hpp
		Core/GlContextState.hpp
		Core/GlContextStateStack.hpp
		Core/GlDebugReportCallback.hpp
		Core/GlDevice.hpp
		Core/GlDisplay.hpp
		Core/GlDisplayMode.hpp
		Core/GlIcdObject.hpp
		Core/GlInstance.hpp
		Core/GlPhysicalDevice.hpp
		Core/GlSurface.hpp
		Core/GlSwapChain.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Core" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Core" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Descriptor/GlDescriptorPool.cpp
		Descriptor/GlDescriptorSet.cpp
		Descriptor/GlDescriptorSetLayout.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Descriptor/GlDescriptorPool.hpp
		Descriptor/GlDescriptorSet.hpp
		Descriptor/GlDescriptorSetLayout.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Descriptor" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Descriptor" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Enum/GlAccessFlag.cpp
		Enum/GlAttachmentPoint.cpp
		Enum/GlAttachmentType.cpp
		Enum/GlBaseType.cpp
		Enum/GlBlendFactor.cpp
		Enum/GlBlendOp.cpp
		Enum/GlBufferDataUsageFlag.cpp
		Enum/GlBufferParameter.cpp
		Enum/GlBufferTarget.cpp
		Enum/GlClearTarget.cpp
		Enum/GlClipInfo.cpp
		Enum/GlCompareOp.cpp
		Enum/GlComponentSwizzle.cpp
		Enum/GlConstantFormat.cpp
		Enum/GlCullModeFlag.cpp
		Enum/GlDebugReportObjectType.cpp
		Enum/GlFenceWaitFlag.cpp
		Enum/GlFenceWaitResult.cpp
		Enum/GlFilter.cpp
		Enum/GlFormat.cpp
		Enum/GlFormatProperty.cpp
		Enum/GlFrameBufferTarget.cpp
		Enum/GlFrontFace.cpp
		Enum/GlGetParameter.cpp
		Enum/GlImageAspectFlag.cpp
		Enum/GlImageLayout.cpp
		Enum/GlImageTiling.cpp
		Enum/GlIndexType.cpp
		Enum/GlLogicOp.cpp
		Enum/GlMemoryBarrierFlag.cpp
		Enum/GlMemoryMapFlag.cpp
		Enum/GlMemoryPropertyFlag.cpp
		Enum/GlMipmapMode.cpp
		Enum/GlPackAlignment.cpp
		Enum/GlPolygonMode.cpp
		Enum/GlPrimitiveTopology.cpp
		Enum/GlQueryResultFlag.cpp
		Enum/GlQueryType.cpp
		Enum/GlSampleCountFlag.cpp
		Enum/GlSamplerParameter.cpp
		Enum/GlShaderInfo.cpp
		Enum/GlShaderStageFlag.cpp
		Enum/GlStencilOp.cpp
		Enum/GlTextureType.cpp
		Enum/GlTextureUnit.cpp
		Enum/GlTextureViewType.cpp
		Enum/GlTweak.cpp
		Enum/GlValueName.cpp
		Enum/GlWrapMode.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Enum/GlAccessFlag.hpp
		Enum/GlAttachmentPoint.hpp
		Enum/GlAttachmentType.hpp
		Enum/GlBaseType.hpp
		Enum/GlBlendFactor.hpp
		Enum/GlBlendOp.hpp
		Enum/GlBufferDataUsageFlag.hpp
		Enum/GlBufferParameter.hpp
		Enum/GlBufferTarget.hpp
		Enum/GlClearTarget.hpp
		Enum/GlClipInfo.hpp
		Enum/GlCompareOp.hpp
		Enum/GlComponentSwizzle.hpp
		Enum/GlConstantFormat.hpp
		Enum/GlCullModeFlag.hpp
		Enum/GlDebugReportObjectType.hpp
		Enum/GlFenceWaitFlag.hpp
		Enum/GlFenceWaitResult.hpp
		Enum/GlFilter.hpp
		Enum/GlFormat.hpp
		Enum/GlFormatProperty.hpp
		Enum/GlFrameBufferTarget.hpp
		Enum/GlFrontFace.hpp
		Enum/GlGetParameter.hpp
		Enum/GlImageAspectFlag.hpp
		Enum/GlImageLayout.hpp
		Enum/GlImageTiling.hpp
		Enum/GlIndexType.hpp
		Enum/GlLogicOp.hpp
		Enum/GlMemoryBarrierFlag.hpp
		Enum/GlMemoryMapFlag.hpp
		Enum/GlMemoryPropertyFlag.hpp
		Enum/GlMipmapMode.hpp
		Enum/GlPackAlignment.hpp
		Enum/GlPolygonMode.hpp
		Enum/GlPrimitiveTopology.hpp
		Enum/GlQueryResultFlag.hpp
		Enum/GlQueryType.hpp
		Enum/GlSampleCountFlag.hpp
		Enum/GlSamplerParameter.hpp
		Enum/GlShaderBinaryFormat.hpp
		Enum/GlShaderInfo.hpp
		Enum/GlShaderStageFlag.hpp
		Enum/GlStencilOp.hpp
		Enum/GlTexLevelParameter.hpp
		Enum/GlTexParameter.hpp
		Enum/GlTextureType.hpp
		Enum/GlTextureUnit.hpp
		Enum/GlTextureViewType.hpp
		Enum/GlTweak.hpp
		Enum/GlValueName.hpp
		Enum/GlWrapMode.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Enum" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Enum" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Image/GlImage.cpp
		Image/GlImageView.cpp
		Image/GlSampler.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Image/GlImage.hpp
		Image/GlImageView.hpp
		Image/GlSampler.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Image" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Image" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Miscellaneous/GlBufferMemoryBinding.cpp
		Miscellaneous/GlDebug.cpp
		Miscellaneous/GlDeviceMemory.cpp
		Miscellaneous/GlDeviceMemoryBinding.cpp
		Miscellaneous/GlExtensionsHandler.cpp
		Miscellaneous/GlImageMemoryBinding.cpp
		Miscellaneous/GlPixelFormat.cpp
		Miscellaneous/GlQueryPool.cpp
		Miscellaneous/GlScreenHelpers.cpp
		Miscellaneous/GlValidator.cpp
		Miscellaneous/GlValidatorInterfaceQuery.cpp
		Miscellaneous/GlValidatorOldStyle.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Miscellaneous/GlBufferMemoryBinding.hpp
		Miscellaneous/GlCallLogger.hpp
		Miscellaneous/GlDebug.hpp
		Miscellaneous/GlDeviceMemory.hpp
		Miscellaneous/GlDeviceMemoryBinding.hpp
		Miscellaneous/GlDummyIndexBuffer.hpp
		Miscellaneous/GlExtensionsHandler.hpp
		Miscellaneous/GlImageMemoryBinding.hpp
		Miscellaneous/GlPixelFormat.hpp
		Miscellaneous/GlQueryPool.hpp
		Miscellaneous/GlScreenHelpers.hpp
		Miscellaneous/GlValidator.hpp
		Miscellaneous/GlValidatorInterfaceQuery.hpp
		Miscellaneous/GlValidatorOldStyle.hpp
		Miscellaneous/OpenGLDefines.hpp
		Miscellaneous/OpenGLFunctionsList.inl
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Miscellaneous" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Miscellaneous" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Pipeline/GlPipeline.cpp
		Pipeline/GlPipelineCache.cpp
		Pipeline/GlPipelineLayout.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Pipeline/GlPipeline.hpp
		Pipeline/GlPipelineCache.hpp
		Pipeline/GlPipelineLayout.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Pipeline" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Pipeline" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Platform/CGlContext.cpp
		Platform/CGlWindow.cpp
		Platform/EglContext.cpp
		Platform/GlEglContext.cpp
		Platform/GlMswContext.cpp
		Platform/GlMswWindow.cpp
		Platform/GlWaylandContext.cpp
		Platform/GlX11Context.cpp
		Platform/GlX11EglContext.cpp
		Platform/GlX11Window.cpp
		Platform/GlXcbContext.cpp
		Platform/GlXcbWindow.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Platform/CGlContext.hpp
		Platform/CGlWindow.hpp
		Platform/EglContext.hpp
		Platform/GlEglContext.hpp
		Platform/GlMswContext.hpp
		Platform/GlMswWindow.hpp
		Platform/GlWaylandContext.hpp
		Platform/GlX11Context.hpp
		Platform/GlX11EglContext.hpp
		Platform/GlX11Window.hpp
		Platform/GlXcbContext.hpp
		Platform/GlXcbWindow.hpp
	)
	if ( APPLE )
	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
		Platform/CGlContext.mm
		Platform/CGlView.mm
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
		Platform/CGlView.hh
	)
	endif ()
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Platform" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Platform" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		RenderPass/GlFrameBuffer.cpp
		RenderPass/GlRenderPass.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		RenderPass/GlFrameBuffer.hpp
		RenderPass/GlRenderPass.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\RenderPass" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\RenderPass" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Shader/GlShaderModule.cpp
		Shader/GlShaderProgram.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Shader/GlShaderDesc.hpp
		Shader/GlShaderModule.hpp
		Shader/GlShaderProgram.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Shader" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Shader" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Sync/GlEvent.cpp
		Sync/GlFence.cpp
		Sync/GlSemaphore.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Sync/GlEvent.hpp
		Sync/GlFence.hpp
		Sync/GlSemaphore.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
	)
	source_group( "Header Files\\Sync" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
	source_group( "Source Files\\Sync" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

	set( ${PROJECT_NAME}_USE_SPIRV_CROSS 0 )

	if ( ASHES_HAS_SPIRV_CROSS )
		set( ${PROJECT_NAME}_USE_SPIRV_CROSS 1 )
	endif ()

	add_library( ${PROJECT_NAME} SHARED
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_NVS_FILES}
		${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES}
		${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES}
	)
	add_library( ashes::${_PROJECT_NAME}
		ALIAS
		${PROJECT_NAME}
	)
	target_include_directories( ${PROJECT_NAME} PRIVATE
		${Vulkan_INCLUDE_DIR}
		${TARGET_INCLUDE_DIRS}
		${CMAKE_CURRENT_BINARY_DIR}
		${SPIRV_CROSS_DIR}
		${Ashes_SOURCE_DIR}/source/ashes
		${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}
	)
	target_link_libraries( ${PROJECT_NAME} PRIVATE
		${Ashes_BINARY_LIBRARIES}
		ashes::RendererCommon
	)
	target_compile_definitions( ${PROJECT_NAME} PRIVATE
		${_PROJECT_NAME}_USE_SPIRV_CROSS=${${PROJECT_NAME}_USE_SPIRV_CROSS}
		${_PROJECT_NAME}_EXPORTS
		${TARGET_CXX_DEFINITIONS}
		_CRT_SECURE_NO_WARNINGS
		GL_SILENCE_DEPRECATION
	)
	target_compile_options( ${PROJECT_NAME} PRIVATE
		${TARGET_CXX_OPTIONS}
	)
	set_target_properties( ${PROJECT_NAME} PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		FOLDER "${Ashes_BASE_DIR}/Renderer"
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_SOVERSION}
	)
	add_target_precompiled_header_ex( ${PROJECT_NAME}
		ashes/renderer
		${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/GlRendererPch.hpp
		${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/GlRendererPch.cpp
		"${CMAKE_CXX_FLAGS}"
		${${PROJECT_NAME}_SRC_FILES}
	)
	ash_update_icd_target( ${PROJECT_NAME} )
	set( ENABLED_RENDERERS
		${ENABLED_RENDERERS}
		${PROJECT_NAME}
		PARENT_SCOPE
	)

	if ( MSVC )
		set( ENABLED_PDBS
			${ENABLED_PDBS}
			$<TARGET_PDB_FILE:${PROJECT_NAME}>
			PARENT_SCOPE
		)
	endif ()
else ()
	if ( NOT OpenGL_FOUND )
		message( SEND_ERROR "OpenGL library not found." )
	endif ()
endif ()
