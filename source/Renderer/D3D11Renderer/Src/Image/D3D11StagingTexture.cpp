/*
This file belongs to RendererLib.
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
		renderer::ImageCreateInfo getCreateInfos( renderer::Format format
			, renderer::Extent3D const & extent )
		{
			renderer::ImageCreateInfo result;
			result.arrayLayers = 1u;
			result.extent = extent;
			result.format = format;
			result.flags = 0u;
			result.imageType = renderer::TextureType::e2D;
			result.mipLevels = 1u;
			result.usage = renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eTransferSrc;
			return result;
		}
	}

	StagingTexture::StagingTexture( Device const & device
		, renderer::Format format
		, renderer::Extent3D const & extent )
		: renderer::StagingTexture{ device, extent }
		, m_device{ device }
		, m_texture{ device, getCreateInfos( format, extent ) }
	{
		m_texture.bindMemory( device.allocateMemory( m_texture.getMemoryRequirements()
			, renderer::MemoryPropertyFlag::eHostVisible ) );
	}

	void StagingTexture::doCopyToStagingTexture( uint8_t const * data
		, renderer::Format format
		, renderer::Extent3D const & extent )const
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

		if ( !dxCheckError( hr, "Map" ) )
		{
			throw renderer::Exception{ renderer::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( mapped.pData
			, data
			, getSize( extent, format ) );
		context->Unmap( image, 0u );
		safeRelease( context );
		m_device.waitIdle();
	}

	void StagingTexture::doCopyStagingToDestination( renderer::CommandBuffer const & commandBuffer
		, renderer::ImageSubresourceLayers const & subresourceLayers
		, renderer::Format format
		, renderer::Offset3D const & offset
		, renderer::Extent3D const & extent
		, renderer::TextureView const & texture )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		assert( extent.depth <= m_texture.getDimensions().depth );
		commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit );

		commandBuffer.copyImage( renderer::ImageCopy
			{
				renderer::ImageSubresourceLayers
				{
					subresourceLayers.aspectMask,
					0u,
					0u,
					1u,
				},
				renderer::Offset3D{},
				subresourceLayers,
				offset,
				extent
			}
			, m_texture
			, renderer::ImageLayout::eUndefined
			, texture.getTexture()
			, renderer::ImageLayout::eUndefined );
		commandBuffer.end();

		auto fence = m_device.createFence( {} );
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( renderer::FenceTimeout );
	}

	void StagingTexture::doCopyDestinationToStaging( renderer::CommandBuffer const & commandBuffer
		, renderer::ImageSubresourceLayers const & subresourceLayers
		, renderer::Format format
		, renderer::Offset3D const & offset
		, renderer::Extent3D const & extent
		, renderer::TextureView const & texture )const
	{
		assert( extent.width <= m_texture.getDimensions().width );
		assert( extent.height <= m_texture.getDimensions().height );
		assert( extent.depth <= m_texture.getDimensions().depth );
		commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.copyImage( renderer::ImageCopy
			{
				renderer::ImageSubresourceLayers
				{
					subresourceLayers.aspectMask,
					0u,
					0u,
					1u,
				},
				renderer::Offset3D{},
				subresourceLayers,
				offset,
				extent
			}
			, texture.getTexture()
			, renderer::ImageLayout::eUndefined
			, m_texture
			, renderer::ImageLayout::eUndefined );
		commandBuffer.end();

		auto fence = m_device.createFence( {} );
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( renderer::FenceTimeout );
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, renderer::Format format
		, renderer::Extent3D const & extent )const
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

		if ( !dxCheckError( hr, "Map" ) )
		{
			throw renderer::Exception{ renderer::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( data
			, mapped.pData
			, getSize( extent, format ) );
		context->Unmap( image, 0u );
		safeRelease( context );
		m_device.waitIdle();
	}
}
