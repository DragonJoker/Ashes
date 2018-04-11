/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Image/Texture.hpp"

#include "Buffer/StagingBuffer.hpp"
#include "Image/ImageSubresource.hpp"
#include "Image/SubresourceLayout.hpp"
#include "Image/TextureView.hpp"
#include "Sync/ImageMemoryBarrier.hpp"

namespace renderer
{
	Texture::Texture( Texture && rhs )
		: m_device{ rhs.m_device }
		, m_imageType{ rhs.m_imageType }
		, m_format{ rhs.m_format }
		, m_dimensions{ rhs.m_dimensions }
		, m_mipLevels{ rhs.m_mipLevels }
		, m_arrayLayers{ rhs.m_arrayLayers }
	{
	}

	Texture & Texture::operator=( Texture && rhs )
	{
		if ( &rhs != this )
		{
			m_imageType = rhs.m_imageType;
			m_format = rhs.m_format;
			m_dimensions = rhs.m_dimensions;
			m_mipLevels = rhs.m_mipLevels;
			m_arrayLayers = rhs.m_arrayLayers;
		}

		return *this;
	}

	Texture::Texture( Device const & device
		, TextureType type
		, Format format
		, Extent3D dimensions
		, uint32_t mipLevels
		, uint32_t arrayLayers )
		: m_device{ device }
		, m_imageType{ type }
		, m_format{ format }
		, m_dimensions{ dimensions }
		, m_mipLevels{ mipLevels }
		, m_arrayLayers{ arrayLayers }
	{
	}

	void Texture::bindMemory( DeviceMemoryPtr memory )
	{
		assert( !m_storage && "A resource can only be bound once to a device memory object." );
		m_storage = std::move( memory );
		doBindMemory();
	}

	Texture::Mapped Texture::lock( uint32_t offset
		, uint32_t size
		, MemoryMapFlags flags )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		Mapped mapped{};
		ImageSubresource subResource{};
		subResource.aspectMask = getAspectMask( getFormat() );
		SubresourceLayout subResourceLayout;
		m_device.getImageSubresourceLayout( *this, subResource, subResourceLayout );

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

	void Texture::invalidate( uint32_t offset
		, uint32_t size )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->invalidate( offset, size );
	}

	void Texture::flush( uint32_t offset
		, uint32_t size )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->flush( offset, size );
	}

	void Texture::unlock()const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->unlock();
	}

	void Texture::generateMipmaps()const
	{
		auto const width = int32_t( getDimensions().width );
		auto const height = int32_t( getDimensions().height );
		auto commandBuffer = m_device.getGraphicsCommandPool().createCommandBuffer();

		if ( commandBuffer->begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			auto srcView = createView( {
				renderer::TextureViewType( getType() ),
				getFormat(),
				renderer::ComponentMapping{},
				{
					renderer::getAspectMask( getFormat() ),
					0,
					1u,
					0u,
					1u
				}
			} );
			commandBuffer->memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eTransfer
				, srcView->makeTransferSource( renderer::ImageLayout::eUndefined, 0u ) );

			// Copy down mips from n-1 to n
			for ( uint32_t i = 1; i < getMipmapLevels(); i++ )
			{
				ImageBlit imageBlit{};

				// Source
				imageBlit.srcSubresource.aspectMask = renderer::ImageAspectFlag::eColour;
				imageBlit.srcSubresource.layerCount = 1;
				imageBlit.srcSubresource.mipLevel = i - 1;
				imageBlit.srcOffset.x = 0;
				imageBlit.srcOffset.y = 0;
				imageBlit.srcOffset.z = 0;
				imageBlit.srcExtent.width = int32_t( width >> ( i - 1 ) );
				imageBlit.srcExtent.height = int32_t( height >> ( i - 1 ) );
				imageBlit.srcExtent.depth = 1;

				// Destination
				imageBlit.dstSubresource.aspectMask = renderer::ImageAspectFlag::eColour;
				imageBlit.dstSubresource.layerCount = 1;
				imageBlit.dstSubresource.mipLevel = i;
				imageBlit.dstOffset.x = 0;
				imageBlit.dstOffset.y = 0;
				imageBlit.dstOffset.z = 0;
				imageBlit.dstExtent.width = int32_t( width >> i );
				imageBlit.dstExtent.height = int32_t( height >> i );
				imageBlit.dstExtent.depth = 1;

				renderer::ImageSubresourceRange mipSubRange
				{
					renderer::ImageAspectFlag::eColour,
					i,
					1u,
					0u,
					1u
				};

				// Transiton current mip level to transfer dest
				renderer::ImageMemoryBarrier dstTransitionBarrier
				{
					0u,
					renderer::AccessFlag::eTransferWrite,
					renderer::ImageLayout::eUndefined,
					renderer::ImageLayout::eTransferDstOptimal,
					~( 0u ),
					~( 0u ),
					*this,
					mipSubRange
				};
				commandBuffer->memoryBarrier( renderer::PipelineStageFlag::eTransfer
					, renderer::PipelineStageFlag::eTransfer
					, dstTransitionBarrier );

				// Blit from previous level
				commandBuffer->blitImage( *this
					, renderer::ImageLayout::eTransferSrcOptimal
					, *this
					, renderer::ImageLayout::eTransferDstOptimal
					, { imageBlit }
					, renderer::Filter::eLinear );

				// Transiton current mip level to transfer source for read in next iteration
				renderer::ImageMemoryBarrier srcTransitionBarrier
				{
					renderer::AccessFlag::eTransferWrite,
					renderer::AccessFlag::eTransferRead,
					renderer::ImageLayout::eTransferDstOptimal,
					renderer::ImageLayout::eTransferSrcOptimal,
					~( 0u ),
					~( 0u ),
					*this,
					mipSubRange
				};
				commandBuffer->memoryBarrier( renderer::PipelineStageFlag::eTransfer
					, renderer::PipelineStageFlag::eTransfer
					, srcTransitionBarrier );
			}

			commandBuffer->end();
			m_device.getGraphicsQueue().submit( *commandBuffer, nullptr );
			m_device.waitIdle();
		}
	}

	TextureViewPtr Texture::createView( TextureViewType type
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
