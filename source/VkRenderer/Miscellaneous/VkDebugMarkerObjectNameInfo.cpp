#include "VkRendererPrerequisites.hpp"

#include "Buffer/VkBuffer.hpp"
#include "Buffer/VkBufferView.hpp"
#include "Buffer/VkUniformBuffer.hpp"
#include "Command/VkCommandBuffer.hpp"
#include "Command/VkCommandPool.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkConnection.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkRenderer.hpp"
#include "Core/VkSwapChain.hpp"
#include "Descriptor/VkDescriptorPool.hpp"
#include "Descriptor/VkDescriptorSet.hpp"
#include "Descriptor/VkDescriptorSetLayout.hpp"
#include "Image/VkSampler.hpp"
#include "Image/VkStagingTexture.hpp"
#include "Image/VkTexture.hpp"
#include "Image/VkTextureView.hpp"
#include "Miscellaneous/VkDeviceMemory.hpp"
#include "Miscellaneous/VkQueryPool.hpp"
#include "Pipeline/VkPipeline.hpp"
#include "Pipeline/VkPipelineLayout.hpp"
#include "RenderPass/VkFrameBuffer.hpp"
#include "RenderPass/VkRenderPass.hpp"
#include "Shader/VkAttribute.hpp"
#include "Shader/VkShaderModule.hpp"
#include "Sync/VkEvent.hpp"
#include "Sync/VkFence.hpp"
#include "Sync/VkSemaphore.hpp"

namespace vk_renderer
{
	namespace
	{
		uint64_t getObject( ashes::DebugReportObjectType objectType
			, void const * object )
		{
			switch ( objectType )
			{
			case ashes::DebugReportObjectType::eInstance:
				return ( uint64_t )VkInstance( *reinterpret_cast< Renderer const * >( object ) );
			case ashes::DebugReportObjectType::ePhysicalDevice:
				return ( uint64_t )VkPhysicalDevice( *reinterpret_cast< PhysicalDevice const * >( object ) );
			case ashes::DebugReportObjectType::eDevice:
				return ( uint64_t )VkDevice( *reinterpret_cast< Device const * >( object ) );
			case ashes::DebugReportObjectType::eQueue:
				return ( uint64_t )VkQueue( *reinterpret_cast< Queue const * >( object ) );
			case ashes::DebugReportObjectType::eSemaphore:
				return ( uint64_t )VkSemaphore( *reinterpret_cast< Semaphore const * >( object ) );
			case ashes::DebugReportObjectType::eCommandBuffer:
				return ( uint64_t )VkCommandBuffer( *reinterpret_cast< CommandBuffer const * >( object ) );
			case ashes::DebugReportObjectType::eFence:
				return ( uint64_t )VkFence( *reinterpret_cast< Fence const * >( object ) );
			case ashes::DebugReportObjectType::eDeviceMemory:
				return ( uint64_t )VkDeviceMemory( *reinterpret_cast< DeviceMemory const * >( object ) );
			case ashes::DebugReportObjectType::eBuffer:
				return ( uint64_t )VkBuffer( *reinterpret_cast< Buffer const * >( object ) );
			case ashes::DebugReportObjectType::eImage:
				return ( uint64_t )VkImage( *reinterpret_cast< Texture const * >( object ) );
			case ashes::DebugReportObjectType::eEvent:
				return ( uint64_t )VkEvent( *reinterpret_cast< Event const * >( object ) );
			case ashes::DebugReportObjectType::eQueryPool:
				return ( uint64_t )VkQueryPool( *reinterpret_cast< QueryPool const * >( object ) );
			case ashes::DebugReportObjectType::eBufferView:
				return ( uint64_t )VkBufferView( *reinterpret_cast< BufferView const * >( object ) );
			case ashes::DebugReportObjectType::eImageView:
				return ( uint64_t )VkImageView( *reinterpret_cast< TextureView const * >( object ) );
			case ashes::DebugReportObjectType::eShaderModule:
				return ( uint64_t )VkShaderModule( *reinterpret_cast< ShaderModule const * >( object ) );
			case ashes::DebugReportObjectType::ePipelineLayout:
				return ( uint64_t )VkPipelineLayout( *reinterpret_cast< PipelineLayout const * >( object ) );
			case ashes::DebugReportObjectType::eRenderPass:
				return ( uint64_t )VkRenderPass( *reinterpret_cast< RenderPass const * >( object ) );
			case ashes::DebugReportObjectType::ePipeline:
				return ( uint64_t )VkPipeline( *reinterpret_cast< Pipeline const * >( object ) );
			case ashes::DebugReportObjectType::eDescriptorSetLayout:
				return ( uint64_t )VkDescriptorSetLayout( *reinterpret_cast< DescriptorSetLayout const * >( object ) );
			case ashes::DebugReportObjectType::eSampler:
				return ( uint64_t )VkSampler( *reinterpret_cast< Sampler const * >( object ) );
			case ashes::DebugReportObjectType::eDescriptorPool:
				return ( uint64_t )VkDescriptorPool( *reinterpret_cast< DescriptorPool const * >( object ) );
			case ashes::DebugReportObjectType::eDescriptorSet:
				return ( uint64_t )VkDescriptorSet( *reinterpret_cast< DescriptorSet const * >( object ) );
			case ashes::DebugReportObjectType::eFramebuffer:
				return ( uint64_t )VkFramebuffer( *reinterpret_cast< FrameBuffer const * >( object ) );
			case ashes::DebugReportObjectType::eCommandPool:
				return ( uint64_t )VkCommandPool( *reinterpret_cast< CommandPool const * >( object ) );
			case ashes::DebugReportObjectType::eSwapchain:
				return ( uint64_t )VkSwapchainKHR( *reinterpret_cast< SwapChain const * >( object ) );
			default:
				return 0ull;
			}
		}
	}

	VkDebugMarkerObjectNameInfoEXT convert( ashes::DebugMarkerObjectNameInfo const & value )
	{
		return VkDebugMarkerObjectNameInfoEXT
		{
			VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT,
			nullptr,
			convert( value.objectType ),
			getObject( value.objectType, value.object ),
			value.objectName.c_str(),
		};
	}
}
