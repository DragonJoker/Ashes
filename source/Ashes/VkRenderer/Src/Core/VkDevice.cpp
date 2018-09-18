/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/VkDevice.hpp"

#include "Buffer/VkBuffer.hpp"
#include "Buffer/VkBufferView.hpp"
#include "Buffer/VkUniformBuffer.hpp"
#include "Command/VkCommandPool.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkConnection.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkRenderer.hpp"
#include "Core/VkSwapChain.hpp"
#include "Descriptor/VkDescriptorPool.hpp"
#include "Descriptor/VkDescriptorSetLayout.hpp"
#include "Image/VkSampler.hpp"
#include "Image/VkStagingTexture.hpp"
#include "Image/VkTexture.hpp"
#include "Image/VkTextureView.hpp"
#include "Miscellaneous/VkDeviceMemory.hpp"
#include "Miscellaneous/VkQueryPool.hpp"
#include "Pipeline/VkPipelineLayout.hpp"
#include "RenderPass/VkRenderPass.hpp"
#include "Shader/VkAttribute.hpp"
#include "Shader/VkShaderModule.hpp"
#include "Sync/VkEvent.hpp"
#include "Sync/VkFence.hpp"
#include "Sync/VkSemaphore.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	Device::Device( Renderer const & renderer
		, ashes::ConnectionPtr && connection )
		: ashes::Device{ renderer, connection->getGpu(), *connection }
		, m_renderer{ renderer }
		, m_connection{ static_cast< Connection * >( connection.release() ) }
		, m_gpu{ static_cast< PhysicalDevice const & >( ashes::Device::getPhysicalDevice() ) }
		, m_enabledFeatures{ convert( m_gpu.getFeatures() ) }
	{
		m_timestampPeriod = m_gpu.getProperties().limits.timestampPeriod;
		std::vector< VkDeviceQueueCreateInfo > queueCreateInfos;
		std::vector< float > queuePriorities = { 1.0f };

		queueCreateInfos.push_back(
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,                 // sType
			nullptr,                                                    // pNext
			0,                                                          // flags
			m_connection->getGraphicsQueueFamilyIndex(),                // queueFamilyIndex
			static_cast< uint32_t >( queuePriorities.size() ),          // queueCount
			queuePriorities.data()                                      // pQueuePriorities
		} );

		if ( m_connection->getPresentQueueFamilyIndex() != m_connection->getGraphicsQueueFamilyIndex() )
		{
			queueCreateInfos.push_back(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,             // sType
				nullptr,                                                // pNext
				0,                                                      // flags
				m_connection->getPresentQueueFamilyIndex(),             // queueFamilyIndex
				static_cast< uint32_t >( queuePriorities.size() ),      // queueCount
				queuePriorities.data()                                  // pQueuePriorities
			} );
		}

		if ( m_connection->getComputeQueueFamilyIndex() != m_connection->getGraphicsQueueFamilyIndex() )
		{
			queueCreateInfos.push_back(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,             // sType
				nullptr,                                                // pNext
				0,                                                      // flags
				m_connection->getComputeQueueFamilyIndex(),             // queueFamilyIndex
				static_cast< uint32_t >( queuePriorities.size() ),      // queueCount
				queuePriorities.data()                                  // pQueuePriorities
			} );
		}

		VkDeviceCreateInfo deviceInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			0,                                                                               // flags
			static_cast< uint32_t >( queueCreateInfos.size() ),                              // queueCreateInfoCount
			queueCreateInfos.data(),                                                         // pQueueCreateInfos
			static_cast< uint32_t >( m_gpu.getLayerNames().size() ),                         // enabledLayerCount
			m_gpu.getLayerNames().empty() ? nullptr : m_gpu.getLayerNames().data(),          // ppEnabledLayerNames
			static_cast< uint32_t >( m_gpu.getExtensionNames().size() ),                     // enabledExtensionCount
			m_gpu.getExtensionNames().empty() ? nullptr : m_gpu.getExtensionNames().data(),  // ppEnabledExtensionNames
			&m_enabledFeatures                                                               // pEnabledFeatures
		};
		DEBUG_DUMP( deviceInfo );

		auto res = renderer.vkCreateDevice( m_gpu, &deviceInfo, nullptr, &m_device );
		checkError( res, "LogicalDevice creation" );

#define VK_LIB_DEVICE_FUNCTION( fun ) fun = reinterpret_cast< PFN_##fun >( renderer.vkGetDeviceProcAddr( m_device, #fun ) );
#include "Miscellaneous/VulkanFunctionsList.inl"

		m_presentQueue = std::make_unique< Queue >( *this, m_connection->getPresentQueueFamilyIndex() );
		m_presentCommandPool = std::make_unique< CommandPool >( *this
			, m_presentQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

		if ( m_connection->getGraphicsQueueFamilyIndex() != m_connection->getPresentQueueFamilyIndex() )
		{
			m_graphicsQueue = std::make_unique< Queue >( *this, m_connection->getPresentQueueFamilyIndex() );
		}
		else
		{
			m_graphicsQueue = std::make_unique< Queue >( *this, m_connection->getGraphicsQueueFamilyIndex() );
		}

		m_graphicsCommandPool = std::make_unique< CommandPool >( *this
			, m_graphicsQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

		if ( m_connection->getGraphicsQueueFamilyIndex() != m_connection->getComputeQueueFamilyIndex() )
		{
			m_computeQueue = std::make_unique< Queue >( *this, m_connection->getComputeQueueFamilyIndex() );
		}
		else
		{
			m_computeQueue = std::make_unique< Queue >( *this, m_connection->getGraphicsQueueFamilyIndex() );
		}

		m_computeCommandPool = std::make_unique< CommandPool >( *this
			, m_computeQueue->getFamilyIndex()
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );
	}

	Device::~Device()
	{
		m_graphicsCommandPool.reset();
		m_graphicsQueue.reset();
		m_presentCommandPool.reset();
		m_presentQueue.reset();
		m_computeCommandPool.reset();
		m_computeQueue.reset();
		vkDestroyDevice( m_device, nullptr );
	}

	void Device::updateSurfaceCapabilities()const
	{
		m_connection->updateSurfaceCapabilities();
	}

	ashes::StagingTexturePtr Device::createStagingTexture( ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		return std::make_unique< StagingTexture >( *this, format, extent );
	}

	ashes::RenderPassPtr Device::createRenderPass( ashes::RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	ashes::PipelineLayoutPtr Device::createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeCRefArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	ashes::DescriptorSetLayoutPtr Device::createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray && bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	ashes::DescriptorPoolPtr Device::createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::DescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this, flags, maxSets, poolSizes );
	}

	ashes::DeviceMemoryPtr Device::allocateMemory( ashes::MemoryRequirements const & requirements
		, ashes::MemoryPropertyFlags flags )const
	{
		return std::make_unique< DeviceMemory >( *this
			, requirements
			, flags );
	}

	ashes::TexturePtr Device::createTexture( ashes::ImageCreateInfo const & createInfo )const
	{
		return std::make_unique< Texture >( *this, createInfo );
	}

	void Device::getImageSubresourceLayout( ashes::Texture const & image
		, ashes::ImageSubresource const & subresource
		, ashes::SubresourceLayout & layout )const
	{
		VkImageSubresource vksubresource = convert( subresource );
		VkSubresourceLayout vklayout;
		vkGetImageSubresourceLayout( m_device
			, static_cast< Texture const & >( image )
			, &vksubresource
			, &vklayout );
		layout = convert( vklayout );
	}

	ashes::SamplerPtr Device::createSampler( ashes::SamplerCreateInfo const & createInfo )const
	{
		return std::make_unique< Sampler >( *this, createInfo );
	}

	ashes::BufferBasePtr Device::createBuffer( uint32_t size
		, ashes::BufferTargets target )const
	{
		return std::make_unique< Buffer >( *this
			, size
			, target );
	}

	ashes::BufferViewPtr Device::createBufferView( ashes::BufferBase const & buffer
		, ashes::Format format
		, uint32_t offset
		, uint32_t range )const
	{
		return std::make_unique< BufferView >( *this
			, static_cast< Buffer const & >( buffer )
			, format
			, offset
			, range );
	}

	ashes::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, ashes::BufferTargets target
		, ashes::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< UniformBuffer >( *this
			, count
			, size
			, target
			, memoryFlags );
	}

	ashes::SwapChainPtr Device::createSwapChain( ashes::Extent2D const & size )const
	{
		ashes::SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, size );
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( std::string{ "Could not create the swap chain:\n" } + exc.what() );
		}
		catch ( ... )
		{
			ashes::Logger::logError( "Could not create the swap chain:\nUnknown error" );
		}

		return result;
	}

	ashes::SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	ashes::FencePtr Device::createFence( ashes::FenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this, flags );
	}

	ashes::EventPtr Device::createEvent()const
	{
		return std::make_unique< Event >( *this );
	}

	ashes::CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, ashes::CommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	ashes::ShaderModulePtr Device::createShaderModule( ashes::ShaderStageFlag stage )const
	{
		return std::make_shared< ShaderModule >( *this, stage );
	}

	ashes::QueryPoolPtr Device::createQueryPool( ashes::QueryType type
		, uint32_t count
		, ashes::QueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, type
			, count
			, pipelineStatistics );
	}

	void Device::waitIdle()const
	{
		checkError( vkDeviceWaitIdle( m_device ), "Device wait idle" );
	}

	ashes::MemoryRequirements Device::getBufferMemoryRequirements( VkBuffer buffer )const
	{
		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements( m_device
			, buffer
			, &requirements );
		ashes::MemoryRequirements result = convert( requirements );
		result.type = ashes::ResourceType::eBuffer;
		return result;
	}

	ashes::MemoryRequirements Device::getImageMemoryRequirements( VkImage image )const
	{
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements( m_device
			, image
			, &requirements );
		ashes::MemoryRequirements result = convert( requirements );
		result.type = ashes::ResourceType::eImage;
		return result;
	}
}
