/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Image/D3D11StagingTexture.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11TextureView.hpp"

#include <Core/Exception.hpp>
#include <Miscellaneous/MemoryRequirements.hpp>
#include <Miscellaneous/Offset2D.hpp>
#include <Sync/BufferMemoryBarrier.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

namespace d3d11_renderer
{
	namespace
	{
		ashes::ImageCreateInfo getCreateInfos( ashes::Format format
			, ashes::Extent2D const & extent )
		{
			ashes::ImageCreateInfo result;
			result.arrayLayers = 1u;
			result.extent = { extent.width, extent.height, 1u };
			result.format = format;
			result.flags = 0u;
			result.imageType = ashes::TextureType::e2D;
			result.mipLevels = 1u;
			result.usage = ashes::ImageUsageFlag::eTransferDst | ashes::ImageUsageFlag::eTransferSrc;
			return result;
		}
	}

	StagingTexture::StagingTexture( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & extent )
		: ashes::StagingTexture{ device, extent }
		, m_device{ device }
		, m_texture{ device, getCreateInfos( format, extent ) }
		, m_storage{ device.allocateMemory( m_texture.getMemoryRequirements()
			, ashes::MemoryPropertyFlag::eHostVisible ) }
	{
		m_texture.bindMemory( m_storage );
	}

	uint8_t * StagingTexture::lock( ashes::Extent2D const & size
		, ashes::MemoryMapFlags flags )const
	{
		return m_storage->lock( 0u
			, size.width * size.height
			, flags );
	}

	void StagingTexture::invalidate( ashes::Extent2D const & size )const
	{
		m_storage->invalidate( 0u
			, size.width * size.height );
	}

	void StagingTexture::flush( ashes::Extent2D const & size )const
	{
		m_storage->flush( 0u
			, size.width * size.height );
	}

	void StagingTexture::unlock()const
	{
		m_storage->unlock();
	}

	void StagingTexture::doCopyToStagingTexture( uint8_t const * data
		, ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		auto blockSize = getBlockSize( format );

		if ( extent.width >= blockSize.block.width
			&& extent.height >= blockSize.block.height )
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			auto image = m_texture.getResource();
			D3D11_MAPPED_SUBRESOURCE mapped;
			auto hr = context->Map( image
				, 0u
				, D3D11_MAP_WRITE
				, 0u
				, &mapped );
			dxCheckError( hr, "Map" );
			auto bufferOut = reinterpret_cast< uint8_t * >( mapped.pData );
			auto bufferIn = reinterpret_cast< uint8_t const * >( data );
			auto sizeOut = mapped.RowPitch;
			auto sizeIn = ( extent.width * blockSize.size ) / blockSize.block.width;

			for ( uint32_t x = 0u; x < extent.height / blockSize.block.height; ++x )
			{
				std::memcpy( bufferOut
					, bufferIn
					, sizeIn );
				bufferOut += sizeOut;
				bufferIn += sizeIn;
			}

			context->Unmap( image, 0u );
			safeRelease( context );
			m_device.waitIdle();
		}
	}

	void StagingTexture::doCopyStagingToDestination( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent2D const & extent
		, ashes::TextureView const & texture )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		auto blockSize = getBlockSize( format );

		if ( extent.width >= blockSize.block.width
			&& extent.height >= blockSize.block.height )
		{
			commandBuffer.copyImage( ashes::ImageCopy
				{
					ashes::ImageSubresourceLayers
					{
						subresourceLayers.aspectMask,
						0u,
						0u,
						1u,
					},
					ashes::Offset3D{},
					subresourceLayers,
					offset,
					{ extent.width, extent.height, 1u }
				}
				, m_texture
				, ashes::ImageLayout::eUndefined
				, texture.getTexture()
				, ashes::ImageLayout::eUndefined );
		}
	}

	void StagingTexture::doCopyDestinationToStaging( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent2D const & extent
		, ashes::TextureView const & texture )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		auto blockSize = getBlockSize( format );

		if ( extent.width >= blockSize.block.width
			&& extent.height >= blockSize.block.height )
		{
			commandBuffer.copyImage( ashes::ImageCopy
				{
					subresourceLayers,
					offset,
					ashes::ImageSubresourceLayers
					{
						subresourceLayers.aspectMask,
						0u,
						0u,
						1u,
					},
					ashes::Offset3D{},
					{ extent.width, extent.height, 1u }
				}
				, texture.getTexture()
				, ashes::ImageLayout::eUndefined
				, m_texture
				, ashes::ImageLayout::eUndefined );
		}
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		auto blockSize = getBlockSize( format );

		if ( extent.width >= blockSize.block.width
			&& extent.height >= blockSize.block.height )
		{
			ID3D11DeviceContext * context;
			m_device.getDevice()->GetImmediateContext( &context );
			auto image = m_texture.getResource();
			D3D11_MAPPED_SUBRESOURCE mapped;
			auto hr = context->Map( image
				, 0u
				, D3D11_MAP_WRITE
				, 0u
				, &mapped );
			dxCheckError( hr, "Map" );
			auto bufferIn = reinterpret_cast< uint8_t const * >( mapped.pData );
			auto bufferOut = reinterpret_cast< uint8_t * >( data );
			auto blockSize = getBlockSize( format );
			auto sizeIn = mapped.RowPitch;
			auto sizeOut = ( extent.width * blockSize.size ) / blockSize.block.width;

			for ( uint32_t x = 0u; x < extent.height / blockSize.block.height; ++x )
			{
				std::memcpy( bufferOut
					, bufferIn
					, sizeIn );
				bufferOut += sizeOut;
				bufferIn += sizeIn;
			}

			context->Unmap( image, 0u );
			safeRelease( context );
			m_device.waitIdle();
		}
	}
}
