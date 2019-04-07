/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/Device.hpp"

#include "AshesPP/Buffer/Buffer.hpp"
#include "AshesPP/Buffer/BufferView.hpp"
#include "AshesPP/Core/Instance.hpp"
#include "AshesPP/Core/SwapChain.hpp"
#include "AshesPP/Descriptor/DescriptorPool.hpp"
#include "AshesPP/Descriptor/DescriptorSetLayout.hpp"
#include "AshesPP/Image/Image.hpp"
#include "AshesPP/Image/Sampler.hpp"
#include "AshesPP/Image/StagingTexture.hpp"
#include "AshesPP/Miscellaneous/QueryPool.hpp"
#include "AshesPP/Pipeline/PipelineLayout.hpp"
#include "AshesPP/RenderPass/RenderPass.hpp"
#include "AshesPP/Shader/ShaderModule.hpp"
#include "AshesPP/Sync/Event.hpp"
#include "AshesPP/Sync/Fence.hpp"
#include "AshesPP/Sync/Semaphore.hpp"
#include "AshesPP/Utils/CallStack.hpp"

#include <AshesRenderer/Util/Exception.hpp>

namespace ashes
{
	Device::Device( Instance const & instance
		, PhysicalDevice const & physicalDevice
		, ashes::DeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_physicalDevice{ physicalDevice }
		, m_createInfos{ std::move( createInfos ) }
		, m_memoryProperties{ m_physicalDevice.getMemoryProperties() }
		, m_properties{ m_physicalDevice.getProperties() }
		, m_features{ m_physicalDevice.getFeatures() }
		, m_queueFamilyProperties{ m_physicalDevice.getQueueFamilyProperties() }
	{
		m_timestampPeriod = m_physicalDevice.getProperties().limits.timestampPeriod;
		DEBUG_DUMP( m_createInfos );
		auto res = m_instance.vkCreateDevice( m_physicalDevice
			, &static_cast< VkDeviceCreateInfo const & >( m_createInfos )
			, nullptr
			, &m_internal );
		checkError( res, "LogicalDevice creation" );

#define VK_LIB_DEVICE_FUNCTION( fun ) vk##fun = reinterpret_cast< PFN_vk##fun >( m_instance.vkGetDeviceProcAddr( m_internal, "vk"#fun ) );
#include <AshesCommon/VulkanFunctionsList.inl>
	}

	Device::~Device()
	{
		vkDestroyDevice( m_internal, nullptr );
		reportRegisteredObjects();
	}

	std::array< float, 16u > Device::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_instance.frustum( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		return m_instance.perspective( radiansFovY, aspect, zNear, zFar );
	}

	std::array< float, 16u > Device::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_instance.ortho( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		return m_instance.infinitePerspective( radiansFovY, aspect, zNear );
	}

	StagingTexturePtr Device::createStagingTexture( VkFormat format
		, VkExtent2D const & extent )const
	{
		return std::make_unique< StagingTexture >( *this, format, extent );
	}

	RenderPassPtr Device::createRenderPass( RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	GraphicsPipelinePtr Device::createPipeline( GraphicsPipelineCreateInfo createInfo )const
	{
		return std::make_unique< GraphicsPipeline >( *this
			, std::move( createInfo ) );
	}

	ComputePipelinePtr Device::createPipeline( ComputePipelineCreateInfo createInfo )const
	{
		return std::make_unique< ComputePipeline >( *this
			, std::move( createInfo ) );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayoutCRefArray const & setLayouts
		, VkPushConstantRangeArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	DescriptorSetLayoutPtr Device::createDescriptorSetLayout( VkDescriptorSetLayoutBindingArray bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	DescriptorPoolPtr Device::createDescriptorPool( VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, VkDescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this, flags, maxSets, poolSizes );
	}

	DeviceMemoryPtr Device::allocateMemory( VkMemoryAllocateInfo allocateInfo )const
	{
		return std::make_unique< DeviceMemory >( *this
			, std::move( allocateInfo ) );
	}

	ImagePtr Device::createImage( ImageCreateInfo createInfo )const
	{
		return std::make_unique< Image >( *this, std::move( createInfo ) );
	}

	void Device::getImageSubresourceLayout( Image const & image
		, VkImageSubresource const & subresource
		, VkSubresourceLayout & layout )const
	{
		vkGetImageSubresourceLayout( m_internal
			, image
			, &subresource
			, &layout );
	}

	SamplerPtr Device::createSampler( SamplerCreateInfo createInfo )const
	{
		return std::make_unique< Sampler >( *this, std::move( createInfo ) );
	}

	BufferBasePtr Device::createBuffer( VkDeviceSize size
		, VkBufferUsageFlags target )const
	{
		return std::make_unique< BufferBase >( *this
			, size
			, target );
	}

	BufferViewPtr Device::createBufferView( BufferBase const & buffer
		, VkFormat format
		, VkDeviceSize offset
		, VkDeviceSize range )const
	{
		return std::make_unique< BufferView >( *this
			, buffer
			, format
			, offset
			, range );
	}

	SwapChainPtr Device::createSwapChain( VkSwapchainCreateInfoKHR createInfo )const
	{
		SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, std::move( createInfo ) );
		}
		catch ( std::exception & exc )
		{
			Logger::logError( std::string{ "Could not create the swap chain:\n" } +exc.what() );
		}
		catch ( ... )
		{
			Logger::logError( "Could not create the swap chain:\nUnknown error" );
		}

		return result;
	}

	SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	FencePtr Device::createFence( VkFenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this, flags );
	}

	EventPtr Device::createEvent()const
	{
		return std::make_unique< Event >( *this );
	}

	CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, VkCommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	ShaderModulePtr Device::createShaderModule( UInt32Array const & shader )const
	{
		return std::make_shared< ShaderModule >( *this, shader );
	}

	QueryPoolPtr Device::createQueryPool( VkQueryType type
		, uint32_t count
		, VkQueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, type
			, count
			, pipelineStatistics );
	}

	void Device::debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
	{
#if LOAD_VALIDATION_LAYERS
		if ( vkDebugMarkerSetObjectNameEXT )
		{
			vkDebugMarkerSetObjectNameEXT( m_internal, &nameInfo );
		}
#endif
	}

	QueuePtr Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		return std::make_unique< Queue >( *this
			, familyIndex
			, index );
	}

	void Device::waitIdle()const
	{
		checkError( vkDeviceWaitIdle( m_internal ), "Device wait idle" );
	}

	VkMemoryRequirements Device::getBufferMemoryRequirements( VkBuffer buffer )const
	{
		VkMemoryRequirements result;
		vkGetBufferMemoryRequirements( m_internal
			, buffer
			, &result );
		return result;
	}

	VkMemoryRequirements Device::getImageMemoryRequirements( VkImage image )const
	{
		VkMemoryRequirements result;
		vkGetImageMemoryRequirements( m_internal
			, image
			, &result );
		return result;
	}

	uint32_t Device::deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements )const
	{
		uint32_t result = 0xFFFFFFFFu;
		bool found{ false };

		// Recherche parmi les types de m�moire la premi�re ayant les propri�t�s voulues.
		uint32_t i{ 0 };

		while ( i < m_memoryProperties.memoryTypeCount && !found )
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
			throw ashes::Exception{ VkResult::VK_ERROR_VALIDATION_FAILED_EXT, "Could not deduce memory type" };
		}

		return result;
	}

	PipelineLayoutPtr Device::createPipelineLayout()const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( VkPushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, VkPushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout
		, VkPushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, VkPushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayoutCRefArray const & layouts )const
	{
		return createPipelineLayout( layouts
			, VkPushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( VkPushConstantRangeArray const & pushConstantRanges )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, pushConstantRanges );
	}

	SamplerPtr Device::createSampler( VkSamplerAddressMode wrapS
		, VkSamplerAddressMode wrapT
		, VkSamplerAddressMode wrapR
		, VkFilter minFilter
		, VkFilter magFilter
		, VkSamplerMipmapMode mipFilter
		, float minLod
		, float maxLod
		, float lodBias
		, VkBorderColor borderColour
		, float maxAnisotropy
		, VkCompareOp compareOp )const
	{
		return createSampler( SamplerCreateInfo
			{
				0u,
				minFilter,
				magFilter,
				mipFilter,
				wrapR,
				wrapT,
				wrapS,
				lodBias,
				maxAnisotropy != 1.0f,
				maxAnisotropy,
				compareOp != VK_COMPARE_OP_ALWAYS ? 1u : 0u,
				compareOp,
				minLod,
				maxLod,
				borderColour,
				false
			} );
	}

#ifndef NDEBUG

	void Device::doRegisterObject( char const * const type, void * object )const
	{
		auto hash = std::hash< void const * >{}( object );
		std::stringstream stream;
		stream << callstack::Backtrace{ 20, 4 };
		m_allocated.emplace( hash
			, ObjectAllocation{
				std::string{ type },
				stream.str()
			} );
	}

	void Device::doUnregisterObject( void * object )const
	{
		auto hash = std::hash< void * >{}( object );
		auto it = m_allocated.find( hash );
		assert( it != m_allocated.end() );
		m_allocated.erase( it );
	}

	void Device::doReportRegisteredObjects()const
	{
		for ( auto & alloc : m_allocated )
		{
			std::stringstream stream;
			stream << "Leaked [" << alloc.second.type << "], allocation stack:\n";
			stream << alloc.second.callstack;
			Logger::logError( stream );
		}
	}

#endif
}
