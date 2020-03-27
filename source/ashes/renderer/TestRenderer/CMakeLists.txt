set( _PROJECT_NAME TestRenderer )
project( ashes${_PROJECT_NAME} )

set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 1 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( PROJECT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
set( PROJECT_SOVERSION "${${PROJECT_NAME}_VERSION_BUILD}" )

	set( ${PROJECT_NAME}_SRC_FILES
		ash_test.cpp
		TestRendererPch.cpp
		TestRendererPrerequisites.cpp
	)
	set( ${PROJECT_NAME}_HDR_FILES
		ashestest_api.hpp
		TestRendererPch.hpp
		TestRendererPrerequisites.hpp
	)
	source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
	source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )

	set( ${PROJECT_NAME}_FOLDER_SRC_FILES
		Buffer/TestBuffer.cpp
		Buffer/TestBufferView.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Buffer/TestBuffer.hpp
		Buffer/TestBufferView.hpp
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
		Command/TestCommandBuffer.cpp
		Command/TestCommandPool.cpp
		Command/TestDummyCommandBuffer.cpp
		Command/TestQueue.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Command/TestCommandBuffer.hpp
		Command/TestCommandPool.hpp
		Command/TestQueue.hpp
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
		Command/Commands/TestCommandBase.cpp
		Command/Commands/TestBeginQueryCommand.cpp
		Command/Commands/TestBeginRenderPassCommand.cpp
		Command/Commands/TestBeginSubpassCommand.cpp
		Command/Commands/TestBindComputePipelineCommand.cpp
		Command/Commands/TestBindDescriptorSetCommand.cpp
		Command/Commands/TestBindIndexBufferCommand.cpp
		Command/Commands/TestBindPipelineCommand.cpp
		Command/Commands/TestBindVertexBuffersCommand.cpp
		Command/Commands/TestBlitImageCommand.cpp
		Command/Commands/TestClearAttachmentsCommand.cpp
		Command/Commands/TestClearColourCommand.cpp
		Command/Commands/TestClearDepthStencilCommand.cpp
		Command/Commands/TestCopyBufferCommand.cpp
		Command/Commands/TestCopyBufferToImageCommand.cpp
		Command/Commands/TestCopyImageCommand.cpp
		Command/Commands/TestCopyImageToBufferCommand.cpp
		Command/Commands/TestDispatchCommand.cpp
		Command/Commands/TestDispatchIndirectCommand.cpp
		Command/Commands/TestDownloadMemoryCommand.cpp
		Command/Commands/TestDrawCommand.cpp
		Command/Commands/TestDrawIndexedCommand.cpp
		Command/Commands/TestDrawIndexedIndirectCommand.cpp
		Command/Commands/TestDrawIndirectCommand.cpp
		Command/Commands/TestEndQueryCommand.cpp
		Command/Commands/TestEndRenderPassCommand.cpp
		Command/Commands/TestEndSubpassCommand.cpp
		Command/Commands/TestExecuteActionsCommand.cpp
		Command/Commands/TestExecuteCommandsCommand.cpp
		Command/Commands/TestGenerateMipsCommand.cpp
		Command/Commands/TestMemoryBarrierCommand.cpp
		Command/Commands/TestPushConstantsCommand.cpp
		Command/Commands/TestResetEventCommand.cpp
		Command/Commands/TestResetQueryPoolCommand.cpp
		Command/Commands/TestScissorCommand.cpp
		Command/Commands/TestSetDepthBiasCommand.cpp
		Command/Commands/TestSetEventCommand.cpp
		Command/Commands/TestSetLineWidthCommand.cpp
		Command/Commands/TestUploadMemoryCommand.cpp
		Command/Commands/TestViewportCommand.cpp
		Command/Commands/TestWaitEventsCommand.cpp
		Command/Commands/TestWriteTimestampCommand.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Command/Commands/TestCommandBase.hpp
		Command/Commands/TestBeginQueryCommand.hpp
		Command/Commands/TestBeginRenderPassCommand.hpp
		Command/Commands/TestBeginSubpassCommand.hpp
		Command/Commands/TestBindComputePipelineCommand.hpp
		Command/Commands/TestBindDescriptorSetCommand.hpp
		Command/Commands/TestBindIndexBufferCommand.hpp
		Command/Commands/TestBindPipelineCommand.hpp
		Command/Commands/TestBindVertexBuffersCommand.hpp
		Command/Commands/TestBlitImageCommand.hpp
		Command/Commands/TestClearAttachmentsCommand.hpp
		Command/Commands/TestClearColourCommand.hpp
		Command/Commands/TestClearDepthStencilCommand.hpp
		Command/Commands/TestCopyBufferCommand.hpp
		Command/Commands/TestCopyBufferToImageCommand.hpp
		Command/Commands/TestCopyImageCommand.hpp
		Command/Commands/TestCopyImageToBufferCommand.hpp
		Command/Commands/TestDispatchCommand.hpp
		Command/Commands/TestDispatchIndirectCommand.hpp
		Command/Commands/TestDownloadMemoryCommand.hpp
		Command/Commands/TestDrawCommand.hpp
		Command/Commands/TestDrawIndexedCommand.hpp
		Command/Commands/TestDrawIndexedIndirectCommand.hpp
		Command/Commands/TestDrawIndirectCommand.hpp
		Command/Commands/TestEndQueryCommand.hpp
		Command/Commands/TestEndRenderPassCommand.hpp
		Command/Commands/TestEndSubpassCommand.hpp
		Command/Commands/TestExecuteActionsCommand.hpp
		Command/Commands/TestExecuteCommandsCommand.hpp
		Command/Commands/TestGenerateMipsCommand.hpp
		Command/Commands/TestMemoryBarrierCommand.hpp
		Command/Commands/TestPushConstantsCommand.hpp
		Command/Commands/TestResetEventCommand.hpp
		Command/Commands/TestResetQueryPoolCommand.hpp
		Command/Commands/TestScissorCommand.hpp
		Command/Commands/TestSetDepthBiasCommand.hpp
		Command/Commands/TestSetEventCommand.hpp
		Command/Commands/TestSetLineWidthCommand.hpp
		Command/Commands/TestUploadMemoryCommand.hpp
		Command/Commands/TestViewportCommand.hpp
		Command/Commands/TestWaitEventsCommand.hpp
		Command/Commands/TestWriteTimestampCommand.hpp
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
		Core/TestDebugReportCallback.cpp
		Core/TestDevice.cpp
		Core/TestDisplay.cpp
		Core/TestDisplayMode.cpp
		Core/TestInstance.cpp
		Core/TestLayer.cpp
		Core/TestPhysicalDevice.cpp
		Core/TestSurface.cpp
		Core/TestSwapChain.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Core/TestDebugReportCallback.hpp
		Core/TestDevice.hpp
		Core/TestDisplay.hpp
		Core/TestDisplayMode.hpp
		Core/TestDummyIndexBuffer.hpp
		Core/TestInstance.hpp
		Core/TestLayer.hpp
		Core/TestPhysicalDevice.hpp
		Core/TestSurface.hpp
		Core/TestSwapChain.hpp
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
		Descriptor/TestDescriptorPool.cpp
		Descriptor/TestDescriptorSet.cpp
		Descriptor/TestDescriptorSetLayout.cpp
		Descriptor/TestWriteDescriptorSet.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Descriptor/TestDescriptorPool.hpp
		Descriptor/TestDescriptorSet.hpp
		Descriptor/TestDescriptorSetLayout.hpp
		Descriptor/TestWriteDescriptorSet.hpp
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
		Image/TestImage.cpp
		Image/TestImageView.cpp
		Image/TestSampler.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Image/TestImage.hpp
		Image/TestImageView.hpp
		Image/TestSampler.hpp
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
		Miscellaneous/TestDeviceMemory.cpp
		Miscellaneous/TestQueryPool.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Miscellaneous/TestDeviceMemory.hpp
		Miscellaneous/TestQueryPool.hpp
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
		Pipeline/TestComputePipeline.cpp
		Pipeline/TestPipeline.cpp
		Pipeline/TestPipelineCache.cpp
		Pipeline/TestPipelineLayout.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Pipeline/TestComputePipeline.hpp
		Pipeline/TestPipeline.hpp
		Pipeline/TestPipelineCache.hpp
		Pipeline/TestPipelineLayout.hpp
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
		RenderPass/TestFrameBuffer.cpp
		RenderPass/TestRenderPass.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		RenderPass/TestFrameBuffer.hpp
		RenderPass/TestRenderPass.hpp
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
		Shader/TestShaderModule.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Shader/TestShaderModule.hpp
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
		Sync/TestEvent.cpp
		Sync/TestFence.cpp
		Sync/TestSemaphore.cpp
	)
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		Sync/TestEvent.hpp
		Sync/TestFence.hpp
		Sync/TestSemaphore.hpp
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

add_library( ${PROJECT_NAME} SHARED
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	${Vulkan_INCLUDE_DIR}
	${TARGET_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${Ashes_SOURCE_DIR}/source/ashes/renderer/${_PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME} PRIVATE
	ashes::RendererCommon
)
target_compile_definitions( ${PROJECT_NAME} PRIVATE
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
set( ENABLED_RENDERERS
	${ENABLED_RENDERERS}
	${PROJECT_NAME}
	PARENT_SCOPE
)
