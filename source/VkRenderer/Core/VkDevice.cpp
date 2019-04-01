/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/VkDevice.hpp"

#include "Buffer/VkBuffer.hpp"
#include "Buffer/VkBufferView.hpp"
#include "Command/VkCommandPool.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkSurface.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkInstance.hpp"
#include "Core/VkSwapChain.hpp"
#include "Descriptor/VkDescriptorPool.hpp"
#include "Descriptor/VkDescriptorSetLayout.hpp"
#include "Image/VkSampler.hpp"
#include "Image/VkImage.hpp"
#include "Image/VkImageView.hpp"
#include "Miscellaneous/VkDeviceMemory.hpp"
#include "Miscellaneous/VkQueryPool.hpp"
#include "Pipeline/VkPipelineLayout.hpp"
#include "RenderPass/VkRenderPass.hpp"
#include "Shader/VkAttribute.hpp"
#include "Shader/VkShaderModule.hpp"
#include "Sync/VkEvent.hpp"
#include "Sync/VkFence.hpp"
#include "Sync/VkSemaphore.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	Device::Device( Instance const & instance
		, PhysicalDevice const & physicalDevice
		, ashes::DeviceCreateInfo createInfos )
		: ashes::Device{ instance
			, physicalDevice
			, std::move( createInfos ) }
		, m_instance{ instance }
		, m_gpu{ physicalDevice }
		, m_vkCreateInfos{ convert( m_createInfos
			, m_queueCreateInfos
			, m_enabledLayerNames
			, m_enabledExtensionNames
			, m_enabledFeatures ) }
		, m_memoryProperties{ m_gpu.getMemoryProperties() }
	{
		m_timestampPeriod = m_gpu.getProperties().limits.timestampPeriod;
		DEBUG_DUMP( m_vkCreateInfos );
		auto res = m_instance.vkCreateDevice( m_gpu, &m_vkCreateInfos, nullptr, &m_device );
		checkError( res, "LogicalDevice creation" );
		doCreateQueues();

#define VK_LIB_DEVICE_FUNCTION( fun ) fun = reinterpret_cast< PFN_##fun >( instance.vkGetDeviceProcAddr( m_device, #fun ) );
#include "Miscellaneous/VulkanFunctionsList.inl"
	}

	Device::~Device()
	{
		vkDestroyDevice( m_device, nullptr );
	}

	ashes::RenderPassPtr Device::createRenderPass( ashes::RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	ashes::PipelineLayoutPtr Device::createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
			, ashes::PushConstantRangeArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	ashes::DescriptorSetLayoutPtr Device::createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	ashes::DescriptorPoolPtr Device::createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::DescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this, flags, maxSets, poolSizes );
	}

	ashes::DeviceMemoryPtr Device::allocateMemory( ashes::MemoryAllocateInfo allocateInfo )const
	{
		return std::make_unique< DeviceMemory >( *this
			, std::move( allocateInfo ) );
	}

	ashes::ImagePtr Device::createImage( ashes::ImageCreateInfo const & createInfo )const
	{
		return std::make_unique< Image >( *this, createInfo );
	}

	void Device::getImageSubresourceLayout( ashes::Image const & image
		, ashes::ImageSubresource const & subresource
		, ashes::SubresourceLayout & layout )const
	{
		VkImageSubresource vksubresource = convert( subresource );
		VkSubresourceLayout vklayout;
		vkGetImageSubresourceLayout( m_device
			, static_cast< Image const & >( image )
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

	ashes::SwapChainPtr Device::createSwapChain( ashes::SwapChainCreateInfo createInfo )const
	{
		ashes::SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, std::move( createInfo ) );
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

	void Device::debugMarkerSetObjectName( ashes::DebugMarkerObjectNameInfo const & nameInfo )const
	{
#if LOAD_VALIDATION_LAYERS
		if ( vkDebugMarkerSetObjectNameEXT )
		{
			auto vkNameInfo = convert( nameInfo );
			vkDebugMarkerSetObjectNameEXT( m_device, &vkNameInfo );
		}
#endif
	}

	ashes::QueuePtr Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		auto it = m_queues.find( familyIndex );

		if ( it == m_queues.end() )
		{
			throw ashes::Exception{ ashes::Result::eErrorRenderer, "Couldn't find family index within created queues" };
		}

		return std::make_unique< Queue >( *this
			, it->second
			, index );
	}

	void Device::waitIdle()const
	{
		checkError( vkDeviceWaitIdle( m_device ), "Device wait idle" );
	}

	uint32_t Device::deduceMemoryType( uint32_t typeBits
		, ashes::MemoryPropertyFlags requirements )const
	{
		uint32_t result = 0xFFFFFFFFu;
		bool found{ false };

		// Recherche parmi les types de m�moire la premi�re ayant les propri�t�s voulues.
		uint32_t i{ 0 };

		while ( i < m_memoryProperties.memoryTypes.size() && !found )
		{
			if ( ( typeBits & 1 ) == 1 )
			{
				// Le type de m�moire est disponible, a-t-il les propri�t�s demand�es?
				if ( ( m_memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					result = i;
					found = true;
				}
			}

			typeBits >>= 1;
			++i;
		}

		if ( !found )
		{
			throw ashes::Exception{ ashes::Result::eErrorRenderer, "Could not deduce memory type" };
		}

		return result;
	}

	void Device::doCreateQueues()
	{
		for ( auto & queueCreateInfo : m_createInfos.queueCreateInfos )
		{
			m_queues.emplace( queueCreateInfo.queueFamilyIndex, queueCreateInfo );
		}
	}
}
