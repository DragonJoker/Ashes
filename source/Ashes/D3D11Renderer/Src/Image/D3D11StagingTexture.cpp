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
#include <Sync/BufferMemoryBarrier.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

namespace d3d11_renderer
{
	namespace
	{
		ashes::ImageCreateInfo getCreateInfos( ashes::Format format
			, ashes::Extent3D const & extent )
		{
			ashes::ImageCreateInfo result;
			result.arrayLayers = 1u;
			result.extent = extent;
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
		, ashes::Extent3D const & extent )
		: ashes::StagingTexture{ device, extent }
		, m_device{ device }
		, m_texture{ device, getCreateInfos( format, extent ) }
	{
		m_texture.bindMemory( device.allocateMemory( m_texture.getMemoryRequirements()
			, ashes::MemoryPropertyFlag::eHostVisible ) );
	}

	void StagingTexture::doCopyToStagingTexture( uint8_t const * data
		, ashes::Format format
		, ashes::Extent3D const & extent )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		assert( extent.depth <= m_texture.getDimensions().depth );
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
		std::memcpy( mapped.pData
			, data
			, getSize( extent, format ) );
		context->Unmap( image, 0u );
		safeRelease( context );
		m_device.waitIdle();
	}

	void StagingTexture::doCopyStagingToDestination( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent3D const & extent
		, ashes::TextureView const & texture )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		assert( extent.depth <= m_texture.getDimensions().depth );
		commandBuffer.begin( ashes::CommandBufferUsageFlag::eOneTimeSubmit );

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
				extent
			}
			, m_texture
			, ashes::ImageLayout::eUndefined
			, texture.getTexture()
			, ashes::ImageLayout::eUndefined );
		commandBuffer.end();

		auto fence = m_device.createFence( {} );
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( ashes::FenceTimeout );
	}

	void StagingTexture::doCopyDestinationToStaging( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent3D const & extent
		, ashes::TextureView const & texture )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		assert( extent.depth <= m_texture.getDimensions().depth );
		commandBuffer.begin( ashes::CommandBufferUsageFlag::eOneTimeSubmit );
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
				extent
			}
			, texture.getTexture()
			, ashes::ImageLayout::eUndefined
			, m_texture
			, ashes::ImageLayout::eUndefined );
		commandBuffer.end();

		auto fence = m_device.createFence( {} );
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( ashes::FenceTimeout );
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, ashes::Format format
		, ashes::Extent3D const & extent )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		assert( extent.depth <= m_texture.getDimensions().depth );
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
		std::memcpy( data
			, mapped.pData
			, getSize( extent, format ) );
		context->Unmap( image, 0u );
		safeRelease( context );
		m_device.waitIdle();
	}
}
