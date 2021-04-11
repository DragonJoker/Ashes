set( _PROJECT_NAME ashespp )
project( ${_PROJECT_NAME} )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} library" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

if ( MSVC )
	set( LIBS
		DbgHelp )
endif ()

set( ${PROJECT_NAME}_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashespp/AshesPPPch.cpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/ashespp.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/AshesPPPch.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/AshesPPPrerequisites.hpp
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HDR_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SRC_FILES} )

set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashespp/Buffer/Buffer.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Buffer/BufferView.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Buffer/PushConstantsBuffer.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Buffer/StagingBuffer.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Buffer/UniformBuffer.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Buffer/VertexBuffer.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/Buffer.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/Buffer.inl
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/BufferView.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/PushConstantsBuffer.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/StagingBuffer.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/StagingBuffer.inl
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/UniformBuffer.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/UniformBuffer.inl
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/VertexBuffer.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Buffer/VertexBuffer.inl
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
	${Ashes_SOURCE_DIR}/source/ashespp/Command/CommandBuffer.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Command/CommandPool.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Command/CommandBuffer.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Command/CommandBufferInheritanceInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Command/CommandPool.hpp
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
	${Ashes_SOURCE_DIR}/source/ashespp/Core/Device.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Core/Instance.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Core/PhysicalDevice.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Core/RendererList.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Core/Surface.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Core/SwapChain.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Core/WindowHandle.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Core/ApplicationInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/Device.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/DeviceCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/DeviceQueueCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/Instance.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/InstanceCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/PhysicalDevice.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/PlatformWindowHandle.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/RendererList.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/Surface.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/SwapChain.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/SwapChainCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Core/WindowHandle.hpp
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

if ( WIN32 )
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		${Ashes_SOURCE_DIR}/include/ashespp/Core/Platform/MswWindowHandle.hpp
	)
elseif ( APPLE )
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		${Ashes_SOURCE_DIR}/include/ashespp/Core/Platform/MacOsWindowHandle.hpp
	)
elseif ( ANDROID )
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		${Ashes_SOURCE_DIR}/include/ashespp/Core/Platform/AndroidWindowHandle.hpp
	)
else ()
	set( ${PROJECT_NAME}_FOLDER_HDR_FILES
		${Ashes_SOURCE_DIR}/include/ashespp/Core/Platform/MirWindowHandle.hpp
		${Ashes_SOURCE_DIR}/include/ashespp/Core/Platform/WaylandWindowHandle.hpp
		${Ashes_SOURCE_DIR}/include/ashespp/Core/Platform/XcbWindowHandle.hpp
		${Ashes_SOURCE_DIR}/include/ashespp/Core/Platform/XWindowHandle.hpp
	)
endif ()

set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Header Files\\Core\\Platform" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )

set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashespp/Descriptor/DescriptorPool.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Descriptor/DescriptorSet.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Descriptor/DescriptorSetLayout.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Descriptor/DescriptorSetPool.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Descriptor/DescriptorPool.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Descriptor/DescriptorSet.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Descriptor/DescriptorSetLayout.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Descriptor/DescriptorSetPool.hpp
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

set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/AccessFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/AttachmentLoadOp.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/AttachmentStoreOp.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/BlendFactor.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/BlendOp.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/BorderColor.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/BufferCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/BufferUsageFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ClipDirection.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ColorComponentFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ColorSpace.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/CommandBufferResetFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/CommandBufferUsageFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/CommandPoolCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/CompareOp.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ComponentSwizzle.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/CompositeAlphaFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ConstantFormat.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/CullModeFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DebugReportFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DebugReportObjectType.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DependencyFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DescriptorPoolCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DescriptorSetLayoutCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DeviceCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DeviceQueueCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/DynamicStateEnable.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/FenceCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/Filter.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/Flags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/FormatFeatureFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/FramebufferCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/FrontFace.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ImageAspectFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ImageCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ImageLayout.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ImageTiling.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ImageType.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ImageUsageFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ImageViewType.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/IndexType.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/InstanceCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/LogicOp.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/MemoryAllocateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/MemoryHeapFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/MemoryMapFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/MemoryPropertyFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ObjectType.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PhysicalDeviceType.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineBindPoint.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineDepthStencilStateCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineDynamicStateCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineMultisampleStateCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineRasterizationStateCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineStageFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineTessellationStateCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PipelineViewportStateCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PolygonMode.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PresentMode.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/PrimitiveTopology.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/QueryControlFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/QueryPipelineStatisticFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/QueryResultFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/QueryType.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/QueueFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/RenderPassCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SampleCountFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SamplerAddressMode.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SamplerMipmapMode.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/ShaderStageFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SharingMode.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/StencilOp.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SubpassContents.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SubpassDescriptionFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SurfaceCounterFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SurfaceTransformFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/SwapChainCreateFlags.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Enum/VertexInputRate.hpp
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Header Files\\Enum" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )

set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashespp/Image/Image.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Image/ImageView.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Image/Sampler.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Image/StagingTexture.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Image/Image.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Image/ImageCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Image/ImageView.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Image/ImageViewCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Image/Sampler.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Image/SamplerCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Image/StagingTexture.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Image/StagingTexture.inl
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
	${Ashes_SOURCE_DIR}/source/ashespp/Miscellaneous/DeviceMemory.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Miscellaneous/Error.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Miscellaneous/Log.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Miscellaneous/QueryPool.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Miscellaneous/Debug.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Miscellaneous/DeviceMemory.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Miscellaneous/Error.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Miscellaneous/Log.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Miscellaneous/QueryPool.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Miscellaneous/QueueShare.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Miscellaneous/RendererFeatures.hpp
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
	${Ashes_SOURCE_DIR}/source/ashespp/Pipeline/ComputePipeline.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Pipeline/GraphicsPipeline.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Pipeline/Pipeline.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Pipeline/PipelineLayout.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/ComputePipeline.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/ComputePipelineCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/GraphicsPipeline.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/GraphicsPipelineCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/Pipeline.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineColorBlendStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineDepthStencilStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineDynamicStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineInputAssemblyStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineLayout.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineMultisampleStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineRasterizationStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineShaderStageCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineTessellationStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineVertexInputStateCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Pipeline/PipelineViewportStateCreateInfo.hpp
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
	${Ashes_SOURCE_DIR}/source/ashespp/RenderPass/FrameBuffer.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/RenderPass/RenderPass.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/RenderPass/FrameBuffer.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/RenderPass/RenderPass.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/RenderPass/RenderPassCreateInfo.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/RenderPass/SubpassDescription.hpp
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
	${Ashes_SOURCE_DIR}/source/ashespp/Shader/ShaderModule.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Shader/ShaderModule.hpp
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
	${Ashes_SOURCE_DIR}/source/ashespp/Sync/Event.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Sync/Fence.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Sync/Queue.cpp
	${Ashes_SOURCE_DIR}/source/ashespp/Sync/Semaphore.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Sync/Event.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Sync/Fence.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Sync/Queue.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Sync/Semaphore.hpp
	${Ashes_SOURCE_DIR}/include/ashespp/Sync/SyncException.hpp
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

set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${Ashes_SOURCE_DIR}/source/ashespp/Utils/CallStack.cpp
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${Ashes_SOURCE_DIR}/include/ashespp/Utils/CallStack.hpp
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Header Files\\Utils" FILES ${${PROJECT_NAME}_FOLDER_HDR_FILES} )
source_group( "Source Files\\Utils" FILES ${${PROJECT_NAME}_FOLDER_SRC_FILES} )

add_library( ${_PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_HDR_FILES}
)
add_library( ashes::${_PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_compile_definitions( ${PROJECT_NAME} PUBLIC
	${Ashes_BINARY_DEFINITIONS}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${Vulkan_INCLUDE_DIR}
		${Ashes_SOURCE_DIR}/source
		${ASHES_CONFIG_DIR}/source
		${ASHES_CONFIG_DIR}/include/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}
		${ASHES_CONFIG_DIR}/source/${_PROJECT_NAME}
		${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ashes::common
	PUBLIC
		ashes::ashes
		${LIBS}
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	FOLDER "${Ashes_BASE_DIR}/Core"
)

add_target_precompiled_header( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME}/AshesPPPch.hpp
	${Ashes_SOURCE_DIR}/source/${_PROJECT_NAME}/AshesPPPch.cpp
	"${CMAKE_CXX_FLAGS}"
	${${PROJECT_NAME}_SRC_FILES}
)

if ( BUILD_ASHES )
	target_add_doc( ${_PROJECT_NAME} "" "*.h *.hpp" )
endif ()

#For libs, we install headers to <install_dir>/include/${PROJECT_NAME}
target_install_headers( ${PROJECT_NAME}
	${Ashes_SOURCE_DIR}/include/${_PROJECT_NAME} )
