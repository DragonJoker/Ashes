/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Image/Image.hpp"

#include "ashespp/Buffer/StagingBuffer.hpp"
#include "ashespp/Command/CommandBuffer.hpp"
#include "ashespp/Core/Device.hpp"
#include "ashespp/Image/ImageView.hpp"
#include "ashespp/Sync/Fence.hpp"

#include <ashes/common/Format.hpp>

namespace ashes
{
	namespace
	{
		VkImageMemoryBarrier makeTransition( VkImage image
			, VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkImageSubresourceRange mipSubRange
			, uint32_t mipLevel
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )
		{
			mipSubRange.baseMipLevel = mipLevel;

			if ( srcQueueFamily == dstQueueFamily )
			{
				srcQueueFamily = VK_QUEUE_FAMILY_IGNORED;
				dstQueueFamily = VK_QUEUE_FAMILY_IGNORED;
			}

			return VkImageMemoryBarrier
			{
				VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				nullptr,
				getAccessMask( srcLayout ),
				getAccessMask( dstLayout ),
				srcLayout,
				dstLayout,
				srcQueueFamily,
				dstQueueFamily,
				image,
				std::move( mipSubRange ),
			};
		}

		VkImageMemoryBarrier makeTransition( VkImage image
			, VkImageLayout prv
			, VkImageLayout cur
			, VkImageSubresourceRange mipSubRange
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )
		{
			auto mipLevel = mipSubRange.baseMipLevel;
			return makeTransition( image
				, prv
				, cur
				, std::move( mipSubRange )
				, mipLevel
				, srcQueueFamily
				, dstQueueFamily );
		}
	}

	VkAccessFlags getAccessMask( VkImageLayout layout )
	{
		VkAccessFlags result{ 0u };

		switch ( layout )
		{
		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
		case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
			result |= VK_ACCESS_MEMORY_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			result |= VK_ACCESS_SHADER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			result |= VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			result |= VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
#ifdef VK_NV_shading_rate_image
		case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
			result |= VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV;
			break;
#endif
#ifdef VK_EXT_fragment_density_map
		case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
			result |= VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;
			break;
#endif
		default:
			break;
		}

		return result;
	}

	VkPipelineStageFlags getStageMask( VkImageLayout layout )
	{
		VkPipelineStageFlags result{ 0u };

		switch ( layout )
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			result |= VK_PIPELINE_STAGE_HOST_BIT;
			break;
		case VK_IMAGE_LAYOUT_GENERAL:
			result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			break;
		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
		case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
			result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			break;
#ifdef VK_EXT_fragment_density_map
		case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
#endif
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			result |= VK_PIPELINE_STAGE_TRANSFER_BIT;
			break;
#ifdef VK_NV_shading_rate_image
		case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
			result |= VK_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV;
			break;
#endif
		default:
			break;
		}

		return result;
	}

	Image::Image()noexcept
		: VkObject{ {} }
	{
	}

	Image::Image( Device const & device
		, ImageCreateInfo createInfo )
		: Image{ device, "Image", std::move( createInfo ) }
	{
	}
	
	Image::Image( Device const & device
		, std::string const & debugName
		, ImageCreateInfo createInfo )
		: VkObject{ debugName }
		, m_device{ &device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( m_createInfo );
		auto res = m_device->vkCreateImage( *m_device
			, &static_cast< VkImageCreateInfo const & >( m_createInfo )
			, m_device->getAllocationCallbacks()
			, &m_internal );
		checkError( res, "Image creation" );
		registerObject( *m_device, debugName, *this );
	}

	Image::Image( Device const & device
		, VkImage image )
		: Image{ device
			, image
			, { 0u
				, VK_IMAGE_TYPE_2D
				, VK_FORMAT_UNDEFINED	// TODO ?
				, VkExtent3D{}			// TODO ?
				, 1u
				, 1u
				, VK_SAMPLE_COUNT_1_BIT
				, VK_IMAGE_TILING_OPTIMAL
				, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT } }
	{
	}

	Image::Image( Device const & device
		, VkImage image
		, ImageCreateInfo createInfo )
		: VkObject{ {} }
		, m_device{ &device }
		, m_createInfo{ std::move( createInfo ) }
		, m_internal{ image }
		, m_ownInternal{ false }
	{
	}

	Image::Image( Device const & device
		, std::string const & debugName
		, VkImage image
		, ImageCreateInfo createInfo )
		: VkObject{ debugName }
		, m_device{ &device }
		, m_createInfo{ std::move( createInfo ) }
		, m_internal{ image }
		, m_ownInternal{ false }
	{
	}

	Image::~Image()noexcept
	{
		assert( ( ( m_internal != VkImage{} ) || m_views.empty() )
			&& "No more internal handle, but some image views remain." );

		if ( m_internal != VkImage{} )
		{
			auto it = m_views.begin();

			while ( it != m_views.end() )
			{
				it = doDestroyView( m_views.begin()->first );
			}

			if ( m_ownInternal )
			{
				unregisterObject( *m_device, *this );
				m_device->vkDestroyImage( *m_device
					, m_internal
					, m_device->getAllocationCallbacks() );
			}
		}
	}

	void Image::bindMemory( DeviceMemoryPtr memory )
	{
		assert( !m_storage && "A resource can only be bound once to a device memory object." );
		m_storage = std::move( memory );
		auto res = m_device->vkBindImageMemory( *m_device
			, m_internal
			, *m_storage
			, 0 );
		checkError( res, "Image storage binding" );
	}

	Image::Mapped Image::lock( uint32_t offset
		, uint32_t size
		, VkMemoryMapFlags flags )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		Mapped mapped{};
		VkImageSubresource subResource{};
		subResource.aspectMask = getAspectMask( getFormat() );
		VkSubresourceLayout subResourceLayout;
		m_device->getImageSubresourceLayout( *this, subResource, subResourceLayout );

		mapped.data = m_storage->lock( offset
			, size
			, flags );

		if ( mapped.data )
		{
			mapped.arrayPitch = subResourceLayout.arrayPitch;
			mapped.depthPitch = subResourceLayout.depthPitch;
			mapped.rowPitch = subResourceLayout.rowPitch;
			mapped.size = subResourceLayout.size;
			mapped.data += subResourceLayout.offset;
		}

		return mapped;
	}

	void Image::invalidate( uint32_t offset
		, uint32_t size )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->invalidate( offset, size );
	}

	void Image::flush( uint32_t offset
		, uint32_t size )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->flush( offset, size );
	}

	void Image::unlock()const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->unlock();
	}

	void Image::generateMipmaps( CommandBuffer const & commandBuffer
		, VkImageLayout srcImageLayout
		, VkImageLayout srcMipsImageLayout
		, VkImageLayout dstImageLayout )const
	{
		generateMipmaps( commandBuffer
			, 0u
			, m_createInfo->arrayLayers
			, srcImageLayout
			, srcMipsImageLayout
			, dstImageLayout );
	}

	void Image::generateMipmaps( CommandPool const & commandPool
		, Queue const & queue
		, VkImageLayout srcImageLayout
		, VkImageLayout srcMipsImageLayout
		, VkImageLayout dstImageLayout )const
	{
		generateMipmaps( commandPool
			, queue
			, 0u
			, m_createInfo->arrayLayers
			, srcImageLayout
			, srcMipsImageLayout
			, dstImageLayout );
	}

	void Image::generateMipmaps( CommandBuffer const & commandBuffer
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, VkImageLayout srcImageLayout
		, VkImageLayout srcMipsImageLayout
		, VkImageLayout dstImageLayout )const
	{
		if ( getMipmapLevels() <= 1u )
		{
			return;
		}

		auto const width = int32_t( getDimensions().width );
		auto const height = int32_t( getDimensions().height );
		auto const depth = int32_t( getDimensions().depth );
		auto const aspectMask = getAspectMask( getFormat() );
		auto const srcStageMask = getStageMask( srcImageLayout );
		auto const srcMipsStageMask = getStageMask( srcMipsImageLayout );
		auto const dstStageMask = getStageMask( dstImageLayout );

		for ( uint32_t i = 0u; i < layerCount; ++i )
		{
			auto layer = baseArrayLayer + i;
			VkImageSubresourceRange mipSubRange
			{
				aspectMask,
				0u,
				1u,
				layer,
				1u
			};
			VkImageBlit imageBlit{};
			imageBlit.dstSubresource.aspectMask = aspectMask;
			imageBlit.dstSubresource.baseArrayLayer = mipSubRange.baseArrayLayer;
			imageBlit.dstSubresource.layerCount = 1;
			imageBlit.dstSubresource.mipLevel = mipSubRange.baseMipLevel;
			imageBlit.dstOffsets[0].x = 0;
			imageBlit.dstOffsets[0].y = 0;
			imageBlit.dstOffsets[0].z = 0;
			imageBlit.dstOffsets[1].x = getSubresourceDimension( width, mipSubRange.baseMipLevel );
			imageBlit.dstOffsets[1].y = getSubresourceDimension( height, mipSubRange.baseMipLevel );
			imageBlit.dstOffsets[1].z = getSubresourceDimension( depth, mipSubRange.baseMipLevel );

			// Transition first mip level to transfer source layout
			commandBuffer.memoryBarrier( srcStageMask
				, VK_PIPELINE_STAGE_TRANSFER_BIT
				, makeTransition( srcImageLayout
					, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
					, mipSubRange ) );

			// Copy down mips
			while ( ++mipSubRange.baseMipLevel < getMipmapLevels() )
			{
				// Blit from previous level
				// Blit source is previous blit destination
				imageBlit.srcSubresource = imageBlit.dstSubresource;
				imageBlit.srcOffsets[0] = imageBlit.dstOffsets[0];
				imageBlit.srcOffsets[1] = imageBlit.dstOffsets[1];

				// Update blit destination
				imageBlit.dstSubresource.mipLevel = mipSubRange.baseMipLevel;
				imageBlit.dstOffsets[1].x = getSubresourceDimension( width, mipSubRange.baseMipLevel );
				imageBlit.dstOffsets[1].y = getSubresourceDimension( height, mipSubRange.baseMipLevel );
				imageBlit.dstOffsets[1].z = getSubresourceDimension( depth, mipSubRange.baseMipLevel );

				// Transition current mip level to transfer dest
				commandBuffer.memoryBarrier( srcMipsStageMask
					, VK_PIPELINE_STAGE_TRANSFER_BIT
					, makeTransition( srcMipsImageLayout
						, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
						, mipSubRange ) );

				// Perform blit
				commandBuffer.blitImage( *this
					, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
					, *this
					, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
					, { imageBlit }
					, VkFilter::VK_FILTER_LINEAR );

				// Transition current mip level to transfer source for read in next iteration
				commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
					, VK_PIPELINE_STAGE_TRANSFER_BIT
					, makeTransition( VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
						, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
						, mipSubRange ) );

				// Transition previous mip level to wanted destination layout
				commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
					, dstStageMask
					, makeTransition( VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
						, dstImageLayout
						, mipSubRange
						, mipSubRange.baseMipLevel - 1u ) );
			}

			// Transition last mip level to wanted destination layout
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, dstStageMask
				, makeTransition( VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
					, dstImageLayout
					, mipSubRange
					, mipSubRange.baseMipLevel - 1u ) );
		}
	}

	void Image::generateMipmaps( CommandPool const & commandPool
		, Queue const & queue
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, VkImageLayout srcImageLayout
		, VkImageLayout srcMipsImageLayout
		, VkImageLayout dstImageLayout )const
	{
		if ( getMipmapLevels() <= 1u )
		{
			return;
		}

		auto commandBuffer = commandPool.createCommandBuffer( "ImageGenMipmaps" );
		commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		generateMipmaps( *commandBuffer
			, baseArrayLayer
			, layerCount
			, srcImageLayout
			, srcMipsImageLayout
			, dstImageLayout );
		commandBuffer->end();
		auto fence = m_device->createFence();
		queue.submit( *commandBuffer, fence.get() );
		fence->wait( MaxTimeout );
	}

	VkMemoryRequirements Image::getMemoryRequirements()const
	{
		return m_device->getImageMemoryRequirements( m_internal );
	}

	ImageView Image::createView( VkImageViewCreateInfo createInfo )const
	{
		return createView( "ImageView"
			, std::move( createInfo ) );
	}

	ImageView Image::createView( std::string const & debugName
		, VkImageViewCreateInfo createInfo )const
	{
		DEBUG_DUMP( createInfo );
		VkImageView vk{};
		auto [it, ins] = m_views.try_emplace( createInfo, vk );
		
		if ( ins )
		{
			auto res = m_device->vkCreateImageView( *m_device
				, &createInfo
				, m_device->getAllocationCallbacks()
				, &it->second );
			checkError( res, "ImageView creation" );
			registerObject( *m_device, debugName, it->second );
		}

		return ImageView{ std::move( createInfo )
			, it->second
			, this };
	}

	ImageView Image::createView( VkImageViewType type
		, VkFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, VkComponentMapping const & mapping )const
	{
		return createView( "ImageView"
			, type
			, format
			, baseMipLevel
			, levelCount
			, baseArrayLayer
			, layerCount
			, mapping );
	}

	ImageView Image::createView( std::string const & debugName
		, VkImageViewType type
		, VkFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, VkComponentMapping const & mapping )const
	{
		return createView( debugName
			, {
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				nullptr,
				0u,
				*this,
				type,
				format,
				mapping,
				{
					getAspectMask( format ),
					baseMipLevel,
					levelCount,
					baseArrayLayer,
					layerCount
				}
			} );
	}

	VkImageMemoryBarrier Image::makeTransition( VkImageLayout srcLayout
		, VkImageLayout dstLayout
		, VkImageSubresourceRange mipSubRange
		, uint32_t mipLevel
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return ashes::makeTransition( *this
			, srcLayout
			, dstLayout
			, mipSubRange
			, mipLevel
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier Image::makeTransition( VkImageLayout srcLayout
		, VkImageLayout dstLayout
		, VkImageSubresourceRange mipSubRange
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return ashes::makeTransition( *this
			, srcLayout
			, dstLayout
			, mipSubRange
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageViewCache::iterator Image::doDestroyView( VkImageViewCreateInfo const & createInfo )const
	{
		auto it = m_views.find( createInfo );

		if ( it != m_views.end() )
		{
			unregisterObject( *m_device, it->second );
			m_device->vkDestroyImageView( *m_device
				, it->second
				, m_device->getAllocationCallbacks() );
			it = m_views.erase( it );
		}

		return it;
	}

	ImageViewCache::iterator Image::doDestroyView( ImageView const & view )const
	{
		return doDestroyView( view.createInfo );
	}
}
