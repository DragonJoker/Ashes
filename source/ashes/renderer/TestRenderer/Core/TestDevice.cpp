/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/TestDevice.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestBufferView.hpp"
#include "Command/TestCommandPool.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestSurface.hpp"
#include "Core/TestDummyIndexBuffer.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Core/TestSwapChain.hpp"
#include "Descriptor/TestDescriptorPool.hpp"
#include "Descriptor/TestDescriptorSetLayout.hpp"
#include "Image/TestSampler.hpp"
#include "Image/TestImage.hpp"
#include "Image/TestImageView.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"
#include "Miscellaneous/TestQueryPool.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "RenderPass/TestRenderPass.hpp"
#include "Shader/TestShaderModule.hpp"
#include "Sync/TestEvent.hpp"
#include "Sync/TestFence.hpp"
#include "Sync/TestSemaphore.hpp"

#include "ashestest_api.hpp"

#include <ashes/common/Hash.hpp>

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <iomanip>
#pragma warning( pop )

namespace ashes::test
{
	namespace
	{
		template< typename T, typename U >
		T getAligned( T value, U align )
		{
			return T( ( value + align - 1 ) & ~( align - 1 ) );
		}

		template< typename T >
		T getSubresourceValue( T value, uint32_t mipLevel )
		{
			return T( value >> mipLevel );
		}

		VkExtent3D getTexelBlockExtent( VkFormat format )
		{
			VkExtent3D texelBlockExtent{ 1u, 1u, 1u };

			if ( ashes::isCompressedFormat( format ) )
			{
				auto extent = ashes::getMinimalExtent2D( format );
				texelBlockExtent.width = extent.width;
				texelBlockExtent.height = extent.height;
			}
			else
			{
				texelBlockExtent.width = 1u;
			}

			return texelBlockExtent;
		}

		uint32_t getTexelBlockByteSize( VkExtent3D const & texelBlockExtent
			, VkFormat format )
		{
			VkDeviceSize texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return uint32_t( texelBlockSize );
		}

		void doCheckEnabledExtensions( VkPhysicalDevice physicalDevice
			, ashes::ArrayView< char const * const > const & extensions )
		{
			auto available = get( physicalDevice )->enumerateExtensionProperties();

			for ( auto & extension : extensions )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&extension]( VkExtensionProperties const & lookup )
					{
						return lookup.extensionName == std::string{ extension };
					} ) )
				{
					throw ExtensionNotPresentException{ extension };
				}
			}
		}

		bool doHasEnabledExtensions( VkPhysicalDevice physicalDevice
			, ashes::ArrayView< char const * const > const & extensions )
		{
			try
			{
				doCheckEnabledExtensions( physicalDevice, extensions );
				return true;
			}
			catch ( ExtensionNotPresentException & )
			{
				return false;
			}
		}

		size_t makeKey( VkImageType type
			, VkFormat format
			, VkExtent3D const & extent
			, uint32_t mipLevels )
		{
			auto result = std::hash< VkImageType >{}( type );
			ashes::hashCombine( result, format );
			ashes::hashCombine( result, extent.width );
			ashes::hashCombine( result, extent.height );
			ashes::hashCombine( result, extent.depth );
			ashes::hashCombine( result, mipLevels );
			return result;
		}
	}

	Device::Device( VkInstance instance
		, VkPhysicalDevice physicalDevice
		, VkDeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_physicalDevice{ physicalDevice }
		, m_createInfos{ std::move( createInfos ) }
	{
		doCreateDummyIndexBuffer();
		doCreateQueues();
	}

	Device::~Device()noexcept
	{
		for ( auto const & [_, creates] : m_queues )
		{
			for ( auto queue : creates.queues )
			{
				deallocate( queue, nullptr );
			}
		}

		for ( auto & [id, image] : m_stagingTextures )
		{
			deallocate( image.first, nullptr );
			deallocate( image.second, nullptr );
		}

		deallocate( m_dummyIndexed.memory, nullptr );
		deallocate( m_dummyIndexed.buffer, nullptr );
	}

	VkPhysicalDeviceLimits const & Device::getLimits()const
	{
		return get( m_physicalDevice )->getProperties().limits;
	}

	bool Device::hasExtension( std::string_view extension )const
	{
		char const * const version = extension.data();
		return doHasEnabledExtensions( m_physicalDevice
			, ashes::makeArrayView( &version, 1u ) );
	}

	VkImage Device::getStagingImage( VkImage image
		, VkDeviceMemory & memory )
	{
		auto key = makeKey( get( image )->getType()
			, get( image )->getFormat()
			, get( image )->getDimensions()
			, get( image )->getMipmapLevels() );
		auto it = m_stagingTextures.find( key );

		if ( it == m_stagingTextures.end() )
		{
			VkImage result{};
			allocate( result
				, nullptr
				, get( this )
				, VkImageCreateInfo
				{
					VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
					nullptr,
					0u,
					get( image )->getType(),
					get( image )->getFormat(),
					get( image )->getDimensions(),
					get( image )->getMipmapLevels(),
					1u, // layerCount, we process images layer per layer.
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_LINEAR,
					VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0u,
					nullptr,
					VK_IMAGE_LAYOUT_UNDEFINED,
				} );
			auto requirements = get( result )->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				, get( m_physicalDevice )->getMemoryProperties() );
			VkDeviceMemory resMem{};
			allocate( resMem
				, nullptr
				, get( this )
				, VkMemoryAllocateInfo
				{
					VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					nullptr,
					requirements.size,
					deduced
				} );
			get( result )->bindMemory( resMem, 0u );
			it = m_stagingTextures.try_emplace( key, result, resMem ).first;
		}

		memory = it->second.second;
		return it->second.first;
	}

	void Device::getImageSubresourceLayout( VkImage image
		, VkImageSubresource const & subresource
		, VkSubresourceLayout & layout )const
	{
		auto extent = getTexelBlockExtent( get( image )->getFormat() );
		auto byteSize = getTexelBlockByteSize( extent, get( image )->getFormat() );
		auto mipWidth = getSubresourceValue( get( image )->getDimensions().width, subresource.mipLevel );
		auto mipHeight = getSubresourceValue( get( image )->getDimensions().height, subresource.mipLevel );
		layout.rowPitch = byteSize * mipWidth / ( extent.width * extent.height * extent.depth );
		layout.arrayPitch = layout.rowPitch * mipHeight * extent.height / ( extent.width * extent.depth );
		layout.depthPitch = layout.arrayPitch;
		layout.offset = subresource.arrayLayer * layout.arrayPitch;
		layout.size = layout.arrayPitch * get( image )->getDimensions().depth;
	}

#if VK_EXT_debug_utils

	VkResult Device::setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const
	{
		switch ( nameInfo.objectType )
		{
		case VK_OBJECT_TYPE_BUFFER:
			get( VkBuffer( nameInfo.objectHandle ) )->setDebugName( nameInfo.pObjectName );
			break;
		case VK_OBJECT_TYPE_IMAGE:
			get( VkImage( nameInfo.objectHandle ) )->setDebugName( nameInfo.pObjectName );
			break;
		default:
			break;
		}

		return VK_SUCCESS;
	}

	VkResult Device::setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & )const
	{
		return VK_SUCCESS;
	}

#endif
#if VK_EXT_debug_marker

	VkResult Device::debugMarkerSetObjectTag( VkDebugMarkerObjectTagInfoEXT const & )const
	{
		return VK_SUCCESS;
	}

	VkResult Device::debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
	{
		switch ( nameInfo.objectType )
		{
		case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT:
			get( VkBuffer( nameInfo.object ) )->setDebugName( nameInfo.pObjectName );
			break;
		case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT:
			get( VkImage( nameInfo.object ) )->setDebugName( nameInfo.pObjectName );
			break;
		default:
			break;
		}

		return VK_SUCCESS;
	}

#endif

	VkQueue Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		auto it = m_queues.find( familyIndex );

		if ( it == m_queues.end() )
		{
			get( m_instance )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "RenderAPI"
				, "Couldn't find family index within created queues" );
			return nullptr;
		}

		if ( it->second.queues.size() <= index )
		{
			get( m_instance )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "RenderAPI"
				, "Couldn't find queue with wanted index within its family" );
			return nullptr;
		}

		return it->second.queues[index];
	}

	VkResult Device::waitIdle()const
	{
		return VK_SUCCESS;
	}

	bool Device::onCopyToImageCommand( VkCommandBuffer cmd
		, VkBufferImageCopyArray const & copyInfo
		, VkBuffer src
		, VkImage dst )const
	{
		return get( m_instance )->onCopyToImageCommand( cmd, copyInfo, src, dst );
	}

	void Device::onReportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )
	{
		get( m_instance )->onReportMessage( flags
			, objectType
			, object
			, location
			, messageCode
			, pLayerPrefix
			, pMessage );
	}

	void Device::doCreateDummyIndexBuffer()
	{
		auto size = uint32_t( sizeof( dummyIndex ) );
		auto count = uint32_t( size / sizeof( dummyIndex[0] ) );
		allocate( m_dummyIndexed.buffer
			, nullptr
			, get( this )
			, VkBufferCreateInfo
			{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,
				0u,
				size,
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u,
				nullptr,
			} );
		auto requirements = get( m_dummyIndexed.buffer )->getMemoryRequirements();
		auto deduced = deduceMemoryType( requirements.memoryTypeBits
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			, get( m_physicalDevice )->getMemoryProperties() );
		allocate( m_dummyIndexed.memory
			, nullptr
			, get( this )
			, VkMemoryAllocateInfo
			{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				requirements.size,
				deduced
			} );
		get( m_dummyIndexed.buffer )->bindMemory( m_dummyIndexed.memory, 0u );
		uint32_t * buffer;

		if ( VK_SUCCESS == get( m_dummyIndexed.memory )->lock( 0u
			, size
			, 0u
			, reinterpret_cast< void ** >( &buffer ) ) )
		{
			std::copy( dummyIndex.data(), dummyIndex.data() + count, buffer );
			get( m_dummyIndexed.memory )->flush( 0, size );
			get( m_dummyIndexed.memory )->unlock();
		}
	}

	void Device::doCreateQueues()
	{
		for ( auto & queueCreateInfo : makeArrayView( m_createInfos.pQueueCreateInfos, m_createInfos.queueCreateInfoCount ) )
		{
			auto it = m_queues.try_emplace( queueCreateInfo.queueFamilyIndex
				, queueCreateInfo ).first;

			for ( auto i = 0u; i < queueCreateInfo.queueCount; ++i )
			{
				VkQueue queue;
				allocate( queue
					, nullptr
					, get( this )
					, it->second.createInfo );
				it->second.queues.emplace_back( queue );
			}
		}
	}
}
