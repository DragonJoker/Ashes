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
#include "Core/VkSurface.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkInstance.hpp"
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

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	namespace
	{
		char const * convert( std::string const & value )
		{
			return value.c_str();
		}

		std::vector< char const * > convert( ashes::StringArray const & values )
		{
			std::vector< char const * > result;

			for ( auto & value : values )
			{
				result.push_back( convert( value ) );
			}

			return result;
		}
	}

	Device::Device( Instance const & instance
		, ashes::SurfacePtr surface
		, ashes::DeviceQueueCreateInfoArray queueCreateInfos
		, ashes::StringArray enabledLayers
		, ashes::StringArray enabledExtensions
		, ashes::PhysicalDeviceFeatures enabledFeatures )
		: ashes::Device{ instance
			, surface->getGpu()
			, *surface
			, std::move( queueCreateInfos )
			, std::move( enabledLayers )
			, std::move( enabledExtensions )
			, std::move( enabledFeatures ) }
		, m_instance{ instance }
		, m_surface{ static_cast< Surface * >( surface.release() ) }
		, m_gpu{ static_cast< PhysicalDevice const & >( ashes::Device::getPhysicalDevice() ) }
		, m_enabledFeatures{ convert( ashes::Device::m_enabledFeatures ) }
	{
		m_timestampPeriod = m_gpu.getProperties().limits.timestampPeriod;
		auto vkQueueCreateInfos = convert< VkDeviceQueueCreateInfo >( m_queueCreateInfos );
		auto vkEnabledLayers = convert( m_enabledLayers );
		auto vkEnabledExtensions = convert( m_enabledExtensions );

		VkDeviceCreateInfo deviceInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			0,                                                                   // flags
			static_cast< uint32_t >( queueCreateInfos.size() ),                  // queueCreateInfoCount
			vkQueueCreateInfos.data(),                                           // pQueueCreateInfos
			static_cast< uint32_t >( vkEnabledLayers.size() ),                   // enabledLayerCount
			vkEnabledLayers.empty() ? nullptr : vkEnabledLayers.data(),          // ppEnabledLayerNames
			static_cast< uint32_t >( vkEnabledExtensions.size() ),               // enabledExtensionCount
			vkEnabledExtensions.empty() ? nullptr : vkEnabledExtensions.data(),  // ppEnabledExtensionNames
			&m_enabledFeatures                                                   // pEnabledFeatures
		};
		DEBUG_DUMP( deviceInfo );

		auto res = m_instance.vkCreateDevice( m_gpu, &deviceInfo, nullptr, &m_device );
		checkError( res, "LogicalDevice creation" );

		doCreateQueues();

#define VK_LIB_DEVICE_FUNCTION( fun ) fun = reinterpret_cast< PFN_##fun >( instance.vkGetDeviceProcAddr( m_device, #fun ) );
#include "Miscellaneous/VulkanFunctionsList.inl"
	}

	Device::~Device()
	{
		vkDestroyDevice( m_device, nullptr );
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

	void Device::doCreateQueues()
	{
		for ( auto & queueCreateInfo : m_queueCreateInfos )
		{
			m_queues.emplace( queueCreateInfo.queueFamilyIndex, queueCreateInfo );
		}
	}
}
