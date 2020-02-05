set( _PROJECT_NAME D3D11Renderer )
project( ashes${_PROJECT_NAME} )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}" )

set( ${PROJECT_NAME}_SRC_FILES
	ash_direct3d11.cpp
	D3D11RendererPch.cpp
	D3D11RendererPrerequisites.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	ashesd3d11_api.hpp
	D3D11RendererPch.hpp
	D3D11RendererPrerequisites.hpp
)
set( ${PROJECT_NAME}_NVS_FILES
	D3D11Renderer.natvis
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )
source_group( "Natvis Files" FILES ${${PROJECT_NAME}_NVS_FILES} )

set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	Buffer/D3D11Buffer.cpp
	Buffer/D3D11BufferView.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Buffer/D3D11Buffer.hpp
	Buffer/D3D11BufferView.hpp
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
	Command/D3D11CommandBuffer.cpp
	Command/D3D11CommandPool.cpp
	Command/D3D11Queue.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Command/D3D11CommandBuffer.hpp
	Command/D3D11CommandPool.hpp
	Command/D3D11Queue.hpp
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
	Command/Commands/D3D11CommandBase.cpp
	Command/Commands/D3D11BeginQueryCommand.cpp
	Command/Commands/D3D11BeginRenderPassCommand.cpp
	Command/Commands/D3D11BeginSubpassCommand.cpp
	Command/Commands/D3D11BindComputePipelineCommand.cpp
	Command/Commands/D3D11BindDescriptorSetCommand.cpp
	Command/Commands/D3D11BindIndexBufferCommand.cpp
	Command/Commands/D3D11BindPipelineCommand.cpp
	Command/Commands/D3D11BindVertexBuffersCommand.cpp
	Command/Commands/D3D11BlitImageCommand.cpp
	Command/Commands/D3D11ClearAttachmentsCommand.cpp
	Command/Commands/D3D11ClearColourCommand.cpp
	Command/Commands/D3D11ClearDepthStencilCommand.cpp
	Command/Commands/D3D11CopyBufferCommand.cpp
	Command/Commands/D3D11CopyBufferToImageCommand.cpp
	Command/Commands/D3D11CopyImageCommand.cpp
	Command/Commands/D3D11CopyImageToBufferCommand.cpp
	Command/Commands/D3D11CopyQueryPoolResultsCommand.cpp
	Command/Commands/D3D11DispatchCommand.cpp
	Command/Commands/D3D11DispatchIndirectCommand.cpp
	Command/Commands/D3D11DownloadMemoryCommand.cpp
	Command/Commands/D3D11DrawCommand.cpp
	Command/Commands/D3D11DrawIndexedCommand.cpp
	Command/Commands/D3D11DrawIndexedIndirectCommand.cpp
	Command/Commands/D3D11DrawIndirectCommand.cpp
	Command/Commands/D3D11EndQueryCommand.cpp
	Command/Commands/D3D11EndRenderPassCommand.cpp
	Command/Commands/D3D11EndSubpassCommand.cpp
	Command/Commands/D3D11ExecuteActionsCommand.cpp
	Command/Commands/D3D11ExecuteCommandsCommand.cpp
	Command/Commands/D3D11FillBufferCommand.cpp
	Command/Commands/D3D11GenerateMipsCommand.cpp
	Command/Commands/D3D11MemoryBarrierCommand.cpp
	Command/Commands/D3D11PushConstantsCommand.cpp
	Command/Commands/D3D11ResetEventCommand.cpp
	Command/Commands/D3D11ResetQueryPoolCommand.cpp
	Command/Commands/D3D11ResolveImageCommand.cpp
	Command/Commands/D3D11ScissorCommand.cpp
	Command/Commands/D3D11SetDepthBiasCommand.cpp
	Command/Commands/D3D11SetEventCommand.cpp
	Command/Commands/D3D11SetLineWidthCommand.cpp
	Command/Commands/D3D11UpdateBufferCommand.cpp
	Command/Commands/D3D11UploadMemoryCommand.cpp
	Command/Commands/D3D11ViewportCommand.cpp
	Command/Commands/D3D11WaitEventsCommand.cpp
	Command/Commands/D3D11WriteTimestampCommand.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Command/Commands/D3D11CommandBase.hpp
	Command/Commands/D3D11BeginQueryCommand.hpp
	Command/Commands/D3D11BeginRenderPassCommand.hpp
	Command/Commands/D3D11BeginSubpassCommand.hpp
	Command/Commands/D3D11BindComputePipelineCommand.hpp
	Command/Commands/D3D11BindDescriptorSetCommand.hpp
	Command/Commands/D3D11BindIndexBufferCommand.hpp
	Command/Commands/D3D11BindPipelineCommand.hpp
	Command/Commands/D3D11BindVertexBuffersCommand.hpp
	Command/Commands/D3D11BlitImageCommand.hpp
	Command/Commands/D3D11ClearAttachmentsCommand.hpp
	Command/Commands/D3D11ClearColourCommand.hpp
	Command/Commands/D3D11ClearDepthStencilCommand.hpp
	Command/Commands/D3D11CopyBufferCommand.hpp
	Command/Commands/D3D11CopyBufferToImageCommand.hpp
	Command/Commands/D3D11CopyImageCommand.hpp
	Command/Commands/D3D11CopyImageToBufferCommand.hpp
	Command/Commands/D3D11CopyQueryPoolResultsCommand.hpp
	Command/Commands/D3D11DispatchCommand.hpp
	Command/Commands/D3D11DispatchIndirectCommand.hpp
	Command/Commands/D3D11DownloadMemoryCommand.hpp
	Command/Commands/D3D11DrawCommand.hpp
	Command/Commands/D3D11DrawIndexedCommand.hpp
	Command/Commands/D3D11DrawIndexedIndirectCommand.hpp
	Command/Commands/D3D11DrawIndirectCommand.hpp
	Command/Commands/D3D11EndQueryCommand.hpp
	Command/Commands/D3D11EndRenderPassCommand.hpp
	Command/Commands/D3D11EndSubpassCommand.hpp
	Command/Commands/D3D11ExecuteActionsCommand.hpp
	Command/Commands/D3D11ExecuteCommandsCommand.hpp
	Command/Commands/D3D11FillBufferCommand.hpp
	Command/Commands/D3D11GenerateMipsCommand.hpp
	Command/Commands/D3D11MemoryBarrierCommand.hpp
	Command/Commands/D3D11PushConstantsCommand.hpp
	Command/Commands/D3D11ResetEventCommand.hpp
	Command/Commands/D3D11ResetQueryPoolCommand.hpp
	Command/Commands/D3D11ResolveImageCommand.hpp
	Command/Commands/D3D11ScissorCommand.hpp
	Command/Commands/D3D11SetDepthBiasCommand.hpp
	Command/Commands/D3D11SetEventCommand.hpp
	Command/Commands/D3D11SetLineWidthCommand.hpp
	Command/Commands/D3D11UpdateBufferCommand.hpp
	Command/Commands/D3D11UploadMemoryCommand.hpp
	Command/Commands/D3D11ViewportCommand.hpp
	Command/Commands/D3D11WaitEventsCommand.hpp
	Command/Commands/D3D11WriteTimestampCommand.hpp
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
	Core/D3D11DebugReportCallback.cpp
	Core/D3D11Device.cpp
	Core/D3D11Display.cpp
	Core/D3D11DisplayMode.cpp
	Core/D3D11Instance.cpp
	Core/D3D11Layer.cpp
	Core/D3D11PhysicalDevice.cpp
	Core/D3D11Surface.cpp
	Core/D3D11SwapChain.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Core/D3D11DebugReportCallback.hpp
	Core/D3D11Device.hpp
	Core/D3D11Display.hpp
	Core/D3D11DisplayMode.hpp
	Core/D3D11DummyIndexBuffer.hpp
	Core/D3D11Instance.hpp
	Core/D3D11Layer.hpp
	Core/D3D11PhysicalDevice.hpp
	Core/D3D11Surface.hpp
	Core/D3D11SwapChain.hpp
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
	Descriptor/D3D11DescriptorPool.cpp
	Descriptor/D3D11DescriptorSet.cpp
	Descriptor/D3D11DescriptorSetLayout.cpp
	Descriptor/D3D11WriteDescriptorSet.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Descriptor/D3D11DescriptorPool.hpp
	Descriptor/D3D11DescriptorSet.hpp
	Descriptor/D3D11DescriptorSetLayout.hpp
	Descriptor/D3D11WriteDescriptorSet.hpp
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
	Enum/D3D11BlendFactor.cpp
	Enum/D3D11BlendOp.cpp
	Enum/D3D11BufferTarget.cpp
	Enum/D3D11CompareOp.cpp
	Enum/D3D11CullModeFlag.cpp
	Enum/D3D11Filter.cpp
	Enum/D3D11Format.cpp
	Enum/D3D11ImageUsageFlag.cpp
	Enum/D3D11IndexType.cpp
	Enum/D3D11MemoryMapFlag.cpp
	Enum/D3D11MemoryPropertyFlag.cpp
	Enum/D3D11PolygonMode.cpp
	Enum/D3D11PrimitiveTopology.cpp
	Enum/D3D11StencilOp.cpp
	Enum/D3D11VertexInputRate.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Enum/D3D11BlendFactor.hpp
	Enum/D3D11BlendOp.hpp
	Enum/D3D11BufferTarget.hpp
	Enum/D3D11CompareOp.hpp
	Enum/D3D11CullModeFlag.hpp
	Enum/D3D11Filter.hpp
	Enum/D3D11Format.hpp
	Enum/D3D11ImageUsageFlag.hpp
	Enum/D3D11IndexType.hpp
	Enum/D3D11MemoryMapFlag.hpp
	Enum/D3D11MemoryPropertyFlag.hpp
	Enum/D3D11PolygonMode.hpp
	Enum/D3D11PrimitiveTopology.hpp
	Enum/D3D11StencilOp.hpp
	Enum/D3D11VertexInputRate.hpp
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
	Image/D3D11Image.cpp
	Image/D3D11ImageView.cpp
	Image/D3D11Sampler.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Image/D3D11Image.hpp
	Image/D3D11ImageView.hpp
	Image/D3D11Sampler.hpp
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
	Miscellaneous/D3D11DeviceMemory.cpp
	Miscellaneous/D3D11QueryPool.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Miscellaneous/D3D11DeviceMemory.hpp
	Miscellaneous/D3D11QueryPool.hpp
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
	Pipeline/D3D11ColourBlendState.cpp
	Pipeline/D3D11ComputePipeline.cpp
	Pipeline/D3D11DepthStencilState.cpp
	Pipeline/D3D11DynamicStates.cpp
	Pipeline/D3D11Pipeline.cpp
	Pipeline/D3D11PipelineCache.cpp
	Pipeline/D3D11PipelineLayout.cpp
	Pipeline/D3D11RasterisationState.cpp
	Pipeline/D3D11Scissor.cpp
	Pipeline/D3D11SpecialisationInfo.cpp
	Pipeline/D3D11StencilOpState.cpp
	Pipeline/D3D11VertexInputState.cpp
	Pipeline/D3D11Viewport.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Pipeline/D3D11ColourBlendState.hpp
	Pipeline/D3D11ComputePipeline.hpp
	Pipeline/D3D11DepthStencilState.hpp
	Pipeline/D3D11DynamicStates.hpp
	Pipeline/D3D11Pipeline.hpp
	Pipeline/D3D11PipelineCache.hpp
	Pipeline/D3D11PipelineLayout.hpp
	Pipeline/D3D11RasterisationState.hpp
	Pipeline/D3D11Scissor.hpp
	Pipeline/D3D11SpecialisationInfo.hpp
	Pipeline/D3D11StencilOpState.hpp
	Pipeline/D3D11VertexInputState.hpp
	Pipeline/D3D11Viewport.hpp
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
	RenderPass/D3D11FrameBuffer.cpp
	RenderPass/D3D11RenderPass.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	RenderPass/D3D11FrameBuffer.hpp
	RenderPass/D3D11RenderPass.hpp
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
	Shader/D3D11ShaderModule.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Shader/D3D11ShaderDesc.hpp
	Shader/D3D11ShaderModule.hpp
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
	Sync/D3D11Event.cpp
	Sync/D3D11Fence.cpp
	Sync/D3D11Semaphore.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	Sync/D3D11Event.hpp
	Sync/D3D11Fence.hpp
	Sync/D3D11Semaphore.hpp
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

if ( ASHES_BUILD_SPIRV_CROSS )
	set( ${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES
		${SPIRV_CROSS_DIR}/spirv_hlsl.cpp
	)
	set( ${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES
		${SPIRV_CROSS_DIR}/spirv_hlsl.hpp
	)
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
		ASHES_D3D11_USE_AMD_AGS
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
		ASHES_D3D11_USE_NVAPI
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
target_include_directories( ${PROJECT_NAME} PRIVATE
	${Vulkan_INCLUDE_DIR}
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${SPIRV_CROSS_DIR}
	${Ashes_SOURCE_DIR}/source/ashes
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}
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
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Renderer"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_SOVERSION}
)
add_target_precompiled_header_ex( ${PROJECT_NAME}
	ashes/renderer
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/${_PROJECT_NAME}Pch.hpp
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}/${_PROJECT_NAME}Pch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
)

parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Buffer "Buffer" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Command "Command" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Command/Commands "Command\\\\Commands" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Core "Core" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Descriptor "Descriptor" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Enum "Enum" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Image "Image" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Miscellaneous "Miscellaneous" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Pipeline "Pipeline" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer RenderPass "RenderPass" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Shader "Shader" )
parse_subdir_files_ex( ${Ashes_SOURCE_DIR} ashes/renderer Sync "Sync" )

source_group( "Header Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_HDR_FILES} )
source_group( "Source Files\\SPIRV-Cross" FILES ${${PROJECT_NAME}_SPIRV_CROSS_CXX_FILES} )
source_group( "Natvis Files" FILES ${${PROJECT_NAME}_NVS_FILES} )

set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
