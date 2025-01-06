set( _PROJECT_NAME XBoxRenderer )
project( ashes${_PROJECT_NAME} )

set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}" )

set( ${PROJECT_NAME}_SRC_FILES
	ash_xbox.cpp
	XBoxRendererPch.cpp
	XBoxRendererPrerequisites.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	ashesxbox_api.hpp
	XBoxRendererPch.hpp
	XBoxRendererPrerequisites.hpp
)
set( ${PROJECT_NAME}_NVS_FILES
	XBoxRenderer.natvis
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )
source_group( "Natvis Files" FILES ${${PROJECT_NAME}_NVS_FILES} )

set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	Buffer/XBoxBuffer.cpp
	Buffer/XBoxBufferView.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Buffer/XBoxBuffer.hpp
	Buffer/XBoxBufferView.hpp
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
	Command/XBoxCommandBuffer.cpp
	Command/XBoxCommandPool.cpp
	Command/XBoxQueue.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Command/XBoxCommandBuffer.hpp
	Command/XBoxCommandPool.hpp
	Command/XBoxQueue.hpp
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
	Command/Commands/XBoxCommandBase.cpp
	Command/Commands/XBoxBeginQueryCommand.cpp
	Command/Commands/XBoxBeginRenderPassCommand.cpp
	Command/Commands/XBoxBeginSubpassCommand.cpp
	Command/Commands/XBoxBindComputePipelineCommand.cpp
	Command/Commands/XBoxBindDescriptorSetCommand.cpp
	Command/Commands/XBoxBindIndexBufferCommand.cpp
	Command/Commands/XBoxBindPipelineCommand.cpp
	Command/Commands/XBoxBindVertexBuffersCommand.cpp
	Command/Commands/XBoxBlitImageCommand.cpp
	Command/Commands/XBoxClearAttachmentsCommand.cpp
	Command/Commands/XBoxClearColourCommand.cpp
	Command/Commands/XBoxClearDepthStencilCommand.cpp
	Command/Commands/XBoxCopyBufferCommand.cpp
	Command/Commands/XBoxCopyBufferToImageCommand.cpp
	Command/Commands/XBoxCopyImageCommand.cpp
	Command/Commands/XBoxCopyImageToBufferCommand.cpp
	Command/Commands/XBoxCopyQueryPoolResultsCommand.cpp
	Command/Commands/XBoxDispatchCommand.cpp
	Command/Commands/XBoxDispatchIndirectCommand.cpp
	Command/Commands/XBoxDownloadMemoryCommand.cpp
	Command/Commands/XBoxDrawCommand.cpp
	Command/Commands/XBoxDrawIndexedCommand.cpp
	Command/Commands/XBoxDrawIndexedIndirectCommand.cpp
	Command/Commands/XBoxDrawIndirectCommand.cpp
	Command/Commands/XBoxEndQueryCommand.cpp
	Command/Commands/XBoxEndRenderPassCommand.cpp
	Command/Commands/XBoxEndSubpassCommand.cpp
	Command/Commands/XBoxExecuteActionsCommand.cpp
	Command/Commands/XBoxExecuteCommandsCommand.cpp
	Command/Commands/XBoxFillBufferCommand.cpp
	Command/Commands/XBoxGenerateMipsCommand.cpp
	Command/Commands/XBoxMemoryBarrierCommand.cpp
	Command/Commands/XBoxPushConstantsCommand.cpp
	Command/Commands/XBoxResetEventCommand.cpp
	Command/Commands/XBoxResetQueryPoolCommand.cpp
	Command/Commands/XBoxResolveImageCommand.cpp
	Command/Commands/XBoxScissorCommand.cpp
	Command/Commands/XBoxSetDepthBiasCommand.cpp
	Command/Commands/XBoxSetEventCommand.cpp
	Command/Commands/XBoxSetLineWidthCommand.cpp
	Command/Commands/XBoxUpdateBufferCommand.cpp
	Command/Commands/XBoxUploadMemoryCommand.cpp
	Command/Commands/XBoxViewportCommand.cpp
	Command/Commands/XBoxWaitEventsCommand.cpp
	Command/Commands/XBoxWriteTimestampCommand.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Command/Commands/XBoxCommandBase.hpp
	Command/Commands/XBoxBeginQueryCommand.hpp
	Command/Commands/XBoxBeginRenderPassCommand.hpp
	Command/Commands/XBoxBeginSubpassCommand.hpp
	Command/Commands/XBoxBindComputePipelineCommand.hpp
	Command/Commands/XBoxBindDescriptorSetCommand.hpp
	Command/Commands/XBoxBindIndexBufferCommand.hpp
	Command/Commands/XBoxBindPipelineCommand.hpp
	Command/Commands/XBoxBindVertexBuffersCommand.hpp
	Command/Commands/XBoxBlitImageCommand.hpp
	Command/Commands/XBoxClearAttachmentsCommand.hpp
	Command/Commands/XBoxClearColourCommand.hpp
	Command/Commands/XBoxClearDepthStencilCommand.hpp
	Command/Commands/XBoxCopyBufferCommand.hpp
	Command/Commands/XBoxCopyBufferToImageCommand.hpp
	Command/Commands/XBoxCopyImageCommand.hpp
	Command/Commands/XBoxCopyImageToBufferCommand.hpp
	Command/Commands/XBoxCopyQueryPoolResultsCommand.hpp
	Command/Commands/XBoxDispatchCommand.hpp
	Command/Commands/XBoxDispatchIndirectCommand.hpp
	Command/Commands/XBoxDownloadMemoryCommand.hpp
	Command/Commands/XBoxDrawCommand.hpp
	Command/Commands/XBoxDrawIndexedCommand.hpp
	Command/Commands/XBoxDrawIndexedIndirectCommand.hpp
	Command/Commands/XBoxDrawIndirectCommand.hpp
	Command/Commands/XBoxEndQueryCommand.hpp
	Command/Commands/XBoxEndRenderPassCommand.hpp
	Command/Commands/XBoxEndSubpassCommand.hpp
	Command/Commands/XBoxExecuteActionsCommand.hpp
	Command/Commands/XBoxExecuteCommandsCommand.hpp
	Command/Commands/XBoxFillBufferCommand.hpp
	Command/Commands/XBoxGenerateMipsCommand.hpp
	Command/Commands/XBoxMemoryBarrierCommand.hpp
	Command/Commands/XBoxPushConstantsCommand.hpp
	Command/Commands/XBoxResetEventCommand.hpp
	Command/Commands/XBoxResetQueryPoolCommand.hpp
	Command/Commands/XBoxResolveImageCommand.hpp
	Command/Commands/XBoxScissorCommand.hpp
	Command/Commands/XBoxSetDepthBiasCommand.hpp
	Command/Commands/XBoxSetEventCommand.hpp
	Command/Commands/XBoxSetLineWidthCommand.hpp
	Command/Commands/XBoxUpdateBufferCommand.hpp
	Command/Commands/XBoxUploadMemoryCommand.hpp
	Command/Commands/XBoxViewportCommand.hpp
	Command/Commands/XBoxWaitEventsCommand.hpp
	Command/Commands/XBoxWriteTimestampCommand.hpp
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
	Core/XBoxDebugReportCallback.cpp
	Core/XBoxDevice.cpp
	Core/XBoxDeviceContextLock.cpp
	Core/XBoxDisplay.cpp
	Core/XBoxDisplayMode.cpp
	Core/XBoxInstance.cpp
	Core/XBoxLayer.cpp
	Core/XBoxPhysicalDevice.cpp
	Core/XBoxSurface.cpp
	Core/XBoxSwapChain.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Core/XBoxDebugReportCallback.hpp
	Core/XBoxDevice.hpp
	Core/XBoxDeviceContextLock.hpp
	Core/XBoxDisplay.hpp
	Core/XBoxDisplayMode.hpp
	Core/XBoxDummyIndexBuffer.hpp
	Core/XBoxInstance.hpp
	Core/XBoxLayer.hpp
	Core/XBoxPhysicalDevice.hpp
	Core/XBoxSurface.hpp
	Core/XBoxSwapChain.hpp
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
	Descriptor/XBoxDescriptorPool.cpp
	Descriptor/XBoxDescriptorSet.cpp
	Descriptor/XBoxDescriptorSetLayout.cpp
	Descriptor/XBoxWriteDescriptorSet.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Descriptor/XBoxDescriptorPool.hpp
	Descriptor/XBoxDescriptorSet.hpp
	Descriptor/XBoxDescriptorSetLayout.hpp
	Descriptor/XBoxWriteDescriptorSet.hpp
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
	Enum/XBoxBlendFactor.cpp
	Enum/XBoxBlendOp.cpp
	Enum/XBoxBufferTarget.cpp
	Enum/XBoxCompareOp.cpp
	Enum/XBoxCullModeFlag.cpp
	Enum/XBoxFilter.cpp
	Enum/XBoxFormat.cpp
	Enum/XBoxImageUsageFlag.cpp
	Enum/XBoxIndexType.cpp
	Enum/XBoxMemoryMapFlag.cpp
	Enum/XBoxMemoryPropertyFlag.cpp
	Enum/XBoxPolygonMode.cpp
	Enum/XBoxPrimitiveTopology.cpp
	Enum/XBoxStencilOp.cpp
	Enum/XBoxVertexInputRate.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Enum/XBoxBlendFactor.hpp
	Enum/XBoxBlendOp.hpp
	Enum/XBoxBufferTarget.hpp
	Enum/XBoxCompareOp.hpp
	Enum/XBoxCullModeFlag.hpp
	Enum/XBoxFilter.hpp
	Enum/XBoxFormat.hpp
	Enum/XBoxImageUsageFlag.hpp
	Enum/XBoxIndexType.hpp
	Enum/XBoxMemoryMapFlag.hpp
	Enum/XBoxMemoryPropertyFlag.hpp
	Enum/XBoxPolygonMode.hpp
	Enum/XBoxPrimitiveTopology.hpp
	Enum/XBoxStencilOp.hpp
	Enum/XBoxVertexInputRate.hpp
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
	Image/XBoxImage.cpp
	Image/XBoxImageView.cpp
	Image/XBoxSampler.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Image/XBoxImage.hpp
	Image/XBoxImageView.hpp
	Image/XBoxSampler.hpp
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
	Miscellaneous/XBoxDeviceMemory.cpp
	Miscellaneous/XBoxQueryPool.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Miscellaneous/XBoxDeviceMemory.hpp
	Miscellaneous/XBoxQueryPool.hpp
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
	Pipeline/XBoxColourBlendState.cpp
	Pipeline/XBoxComputePipeline.cpp
	Pipeline/XBoxDepthStencilState.cpp
	Pipeline/XBoxDynamicStates.cpp
	Pipeline/XBoxPipeline.cpp
	Pipeline/XBoxPipelineCache.cpp
	Pipeline/XBoxPipelineLayout.cpp
	Pipeline/XBoxRasterisationState.cpp
	Pipeline/XBoxScissor.cpp
	Pipeline/XBoxSpecialisationInfo.cpp
	Pipeline/XBoxStencilOpState.cpp
	Pipeline/XBoxVertexInputState.cpp
	Pipeline/XBoxViewport.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Pipeline/XBoxColourBlendState.hpp
	Pipeline/XBoxComputePipeline.hpp
	Pipeline/XBoxDepthStencilState.hpp
	Pipeline/XBoxDynamicStates.hpp
	Pipeline/XBoxPipeline.hpp
	Pipeline/XBoxPipelineCache.hpp
	Pipeline/XBoxPipelineLayout.hpp
	Pipeline/XBoxRasterisationState.hpp
	Pipeline/XBoxScissor.hpp
	Pipeline/XBoxSpecialisationInfo.hpp
	Pipeline/XBoxStencilOpState.hpp
	Pipeline/XBoxVertexInputState.hpp
	Pipeline/XBoxViewport.hpp
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
	RenderPass/XBoxFrameBuffer.cpp
	RenderPass/XBoxRenderPass.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	RenderPass/XBoxFrameBuffer.hpp
	RenderPass/XBoxRenderPass.hpp
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
	Shader/XBoxShaderModule.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Shader/XBoxShaderDesc.hpp
	Shader/XBoxShaderModule.hpp
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
	Sync/XBoxEvent.cpp
	Sync/XBoxFence.cpp
	Sync/XBoxSemaphore.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Sync/XBoxEvent.hpp
	Sync/XBoxFence.hpp
	Sync/XBoxSemaphore.hpp
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

if ( EXISTS ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/inc/ )
	set( AGS_INCLUDE_DIR ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/inc )
	if ( MSVC )
		if ( MSVC15 )
			set( AGS_LIBRARY
				debug ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2015_MDd.lib
				optimized ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2015_MD.lib
			)
		else ()
			set( AGS_LIBRARY
				debug ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2017_MDd.lib
				optimized ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64_2017_MD.lib
			)
		endif ()
	else ()
		set( AGS_LIBRARY ${Ashes_SOURCE_DIR}/external/AGS_SDK/ags_lib/lib/amd_ags_x64.lib )
	endif ()
	set( TARGET_CXX_DEFINITIONS
		${TARGET_CXX_DEFINITIONS}
		ASHES_XBox_USE_AMD_AGS
	)
	set( TARGET_INCLUDE_DIRS
		${TARGET_INCLUDE_DIRS}
		${AGS_INCLUDE_DIR}
	)
	set( Ashes_BINARY_LIBRARIES
		${Ashes_BINARY_LIBRARIES}
		${AGS_LIBRARY}
	)
endif ()

find_package( NVAPI )

if ( NVAPI_FOUND )
	set( TARGET_CXX_DEFINITIONS
		${TARGET_CXX_DEFINITIONS}
		ASHES_XBox_USE_NVAPI
	)
	set( TARGET_INCLUDE_DIRS
		${TARGET_INCLUDE_DIRS}
		${NVAPI_INCLUDE_DIR}
	)
	set( Ashes_BINARY_LIBRARIES
		${Ashes_BINARY_LIBRARIES}
		${NVAPI_LIBRARY}
	)
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
target_sources( ${PROJECT_NAME} 
	PRIVATE
		${Ashes_SOURCE_DIR}/.editorconfig
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${Ashes_SOURCE_DIR}/source/ashes
		${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	SYSTEM PRIVATE
		${Vulkan_INCLUDE_DIR}
		${SPIRV_CROSS_DIR}
		${TARGET_INCLUDE_DIRS}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	ashes::RendererCommon
	d3d11
	dxgi
	d3dcompiler
	${SPIRV_CROSS_HLSL_LIBRARIES}
	${Ashes_BINARY_LIBRARIES}
	${DirectXTex}
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
	${_PROJECT_NAME}_USE_SPIRV_CROSS=${${PROJECT_NAME}_USE_SPIRV_CROSS}
	${_PROJECT_NAME}_EXPORTS
	${TARGET_CXX_DEFINITIONS}
)
target_compile_options( ${PROJECT_NAME} PRIVATE
	${TARGET_CXX_OPTIONS}
)
target_add_compilation_flags( ${PROJECT_NAME} )
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Renderer"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_SOVERSION}
	DEBUG_POSTFIX "d"
)
add_target_precompiled_header_ex( ${PROJECT_NAME}
	ashes/renderer
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/${_PROJECT_NAME}Pch.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/${_PROJECT_NAME}Pch.cpp
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
