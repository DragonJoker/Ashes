/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Image/Image.hpp"

#include "Ashes/Buffer/StagingBuffer.hpp"
#include "Ashes/Command/CommandBuffer.hpp"
#include "Ashes/Core/Device.hpp"
#include "Ashes/Image/ImageSubresource.hpp"
#include "Ashes/Image/SubresourceLayout.hpp"
#include "Ashes/Image/ImageView.hpp"
#include "Ashes/Sync/Fence.hpp"
#include "Ashes/Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
	Image::Image( Image && rhs )
		: m_device{ rhs.m_device }
		, m_flags{ rhs.m_flags }
		, m_imageType{ rhs.m_imageType }
		, m_format{ rhs.m_format }
		, m_dimensions{ rhs.m_dimensions }
		, m_mipLevels{ rhs.m_mipLevels }
		, m_arrayLayers{ rhs.m_arrayLayers }
	{
		registerObject( m_device, "Image", this );
	}

	Image::~Image()
	{
		unregisterObject( m_device, this );
	}

	Image & Image::operator=( Image && rhs )
	{
		if ( &rhs != this )
		{
			m_flags = rhs.m_flags;
			m_imageType = rhs.m_imageType;
			m_format = rhs.m_format;
			m_dimensions = rhs.m_dimensions;
			m_mipLevels = rhs.m_mipLevels;
			m_arrayLayers = rhs.m_arrayLayers;
			registerObject( m_device, "Image", this );
		}

		return *this;
	}

	Image::Image( Device const & device
		, ImageCreateFlags flags
		, ImageType type
		, Format format
		, Extent3D dimensions
		, uint32_t mipLevels
		, uint32_t arrayLayers )
		: m_device{ device }
		, m_flags{ flags }
		, m_imageType{ type }
		, m_format{ format }
		, m_dimensions{ dimensions }
		, m_mipLevels{ mipLevels }
		, m_arrayLayers{ arrayLayers }
	{
		registerObject( m_device, "Image", this );
	}

	void Image::bindMemory( DeviceMemoryPtr memory )
	{
		assert( !m_storage && "A resource can only be bound once to a device memory object." );
		m_storage = std::move( memory );
		doBindMemory();
	}

	uint8_t * Image::lock( SubresourceLayout const & subResourceLayout
		, MemoryMapFlags flags )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->lock( subResourceLayout.offset
			, subResourceLayout.size
			, flags );
	}

	void Image::invalidate( SubresourceLayout const & subResourceLayout )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		m_storage->invalidate( subResourceLayout.offset
			, subResourceLayout.size );
	}

	void Image::flush( SubresourceLayout const & subResourceLayout )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		m_storage->flush( subResourceLayout.offset
			, subResourceLayout.size );
	}

	void Image::unlock()const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		m_storage->unlock();
	}

	void Image::generateMipmaps( CommandPool const & commandPool
		, Queue const & queue )const
	{
		auto commandBuffer = commandPool.createCommandBuffer();
		commandBuffer->begin( ashes::CommandBufferUsageFlag::eOneTimeSubmit );
		generateMipmaps( *commandBuffer );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer, fence.get() );
		fence->wait( FenceTimeout );
	}

	void Image::generateMipmaps( CommandBuffer & commandBuffer )const
	{
		auto const width = int32_t( getDimensions().width );
		auto const height = int32_t( getDimensions().height );

		for ( uint32_t layer = 0u; layer < m_arrayLayers; ++layer )
		{
			auto srcView = createView(
				{
					ashes::ImageViewType( getType() ),
					getFormat(),
					ashes::ComponentMapping{},
					ashes::ImageSubresourceRange
					{
						ashes::getAspectMask( getFormat() ),
						0,
						1u,
						layer,
						1u
					}
				} );
			commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
				, ashes::PipelineStageFlag::eTransfer
				, srcView->makeTransferSource( ashes::ImageLayout::eUndefined, 0u ) );

			// Copy down mips from n-1 to n
			for ( uint32_t level = 1; level < getMipmapLevels(); level++ )
			{
				ImageBlit imageBlit{};

				// Source
				imageBlit.srcSubresource.aspectMask = ashes::ImageAspectFlag::eColour;
				imageBlit.srcSubresource.baseArrayLayer = layer;
				imageBlit.srcSubresource.layerCount = 1;
				imageBlit.srcSubresource.mipLevel = level - 1;
				imageBlit.srcOffset.x = 0;
				imageBlit.srcOffset.y = 0;
				imageBlit.srcOffset.z = 0;
				imageBlit.srcExtent.width = int32_t( width >> ( level - 1 ) );
				imageBlit.srcExtent.height = int32_t( height >> ( level - 1 ) );
				imageBlit.srcExtent.depth = 1;

				// Destination
				imageBlit.dstSubresource.aspectMask = ashes::ImageAspectFlag::eColour;
				imageBlit.dstSubresource.baseArrayLayer = layer;
				imageBlit.dstSubresource.layerCount = 1;
				imageBlit.dstSubresource.mipLevel = level;
				imageBlit.dstOffset.x = 0;
				imageBlit.dstOffset.y = 0;
				imageBlit.dstOffset.z = 0;
				imageBlit.dstExtent.width = int32_t( width >> level );
				imageBlit.dstExtent.height = int32_t( height >> level );
				imageBlit.dstExtent.depth = 1;

				ashes::ImageSubresourceRange mipSubRange
				{
					ashes::ImageAspectFlag::eColour,
					level,
					1u,
					layer,
					1u
				};

				// Transiton current mip level to transfer dest
				ashes::ImageMemoryBarrier dstTransitionBarrier
				{
					0u,
					ashes::AccessFlag::eTransferWrite,
					ashes::ImageLayout::eUndefined,
					ashes::ImageLayout::eTransferDstOptimal,
					~( 0u ),
					~( 0u ),
					*this,
					mipSubRange
				};
				commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
					, ashes::PipelineStageFlag::eTransfer
					, dstTransitionBarrier );

				// Blit from previous level
				commandBuffer.blitImage( *this
					, ashes::ImageLayout::eTransferSrcOptimal
					, *this
					, ashes::ImageLayout::eTransferDstOptimal
					, { imageBlit }
					, ashes::Filter::eLinear );

				// Transiton current mip level to transfer source for read in next iteration
				ashes::ImageMemoryBarrier srcTransitionBarrier
				{
					ashes::AccessFlag::eTransferWrite,
					ashes::AccessFlag::eTransferRead,
					ashes::ImageLayout::eTransferDstOptimal,
					ashes::ImageLayout::eTransferSrcOptimal,
					~( 0u ),
					~( 0u ),
					*this,
					mipSubRange
				};
				commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
					, ashes::PipelineStageFlag::eTransfer
					, srcTransitionBarrier );
			}

			srcView = createView(
				{
					ashes::ImageViewType( getType() ),
					getFormat(),
					ashes::ComponentMapping{},
					ashes::ImageSubresourceRange
					{
						ashes::getAspectMask( getFormat() ),
						0,
						getMipmapLevels(),
						layer,
						1u
					}
				} );
			commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
				, ashes::PipelineStageFlag::eFragmentShader
				, srcView->makeShaderInputResource( ashes::ImageLayout::eUndefined, 0u ) );
		}
	}

	ImageViewPtr Image::createView( ImageViewType type
		, Format format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, ComponentMapping const & mapping )const
	{
		return createView(
		{
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
}
