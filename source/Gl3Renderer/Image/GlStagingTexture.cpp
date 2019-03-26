/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Image/GlStagingTexture.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"

#include <Ashes/Core/Exception.hpp>
#include <Ashes/Miscellaneous/MemoryRequirements.hpp>
#include <Ashes/Sync/BufferMemoryBarrier.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>

namespace gl_renderer
{
	namespace
	{
		uint32_t getSize( ashes::Extent2D const & extent )
		{
			return std::max( 1u, extent.width )
				* std::max( 1u, extent.height );
		}

		ashes::MemoryAllocateInfo getAllocateInfo( Buffer const & buffer )
		{
			auto requirements = buffer.getMemoryRequirements();
			auto deduced = deduceMemoryType( requirements.memoryTypeBits
				, ashes::MemoryPropertyFlag::eHostVisible );
			return { requirements.size, deduced };
		}
	}

	StagingTexture::StagingTexture( Device const & device
		, ashes::Format format
		, ashes::Extent2D const & extent )
		: ashes::StagingTexture{ device, extent }
		, m_device{ device }
		, m_buffer{ device, getSize( extent, format ), ashes::BufferTarget::eTransferDst | ashes::BufferTarget::eTransferSrc }
		, m_storage{ device.allocateMemory( getAllocateInfo( m_buffer ) ) }
	{
		m_buffer.bindMemory( m_storage );
	}

	void StagingTexture::doCopyToStagingTexture( uint8_t const * data
		, ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		uint32_t size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, size
			, ashes::MemoryMapFlag::eWrite | ashes::MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw ashes::Exception{ ashes::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size );
		m_buffer.flush( 0u, size );
		m_buffer.unlock();
		m_device.waitIdle();
	}

	void StagingTexture::doCopyStagingToDestination( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent2D const & extent
		, ashes::ImageView const & texture )const
	{
		uint32_t size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
		commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eTransfer
			, m_buffer.makeTransferSource() );
		commandBuffer.copyToImage( ashes::BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				ashes::Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					1u
				}
			}
			, m_buffer
			, texture.getImage() );
	}

	void StagingTexture::doCopyDestinationToStaging( ashes::CommandBuffer const & commandBuffer
		, ashes::ImageSubresourceLayers const & subresourceLayers
		, ashes::Format format
		, ashes::Offset3D const & offset
		, ashes::Extent2D const & extent
		, ashes::ImageView const & texture )const
	{
		uint32_t size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
		commandBuffer.copyToBuffer( ashes::BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				ashes::Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					1u
				}
			}
			, texture.getImage()
			, m_buffer );
	}

	void StagingTexture::doCopyFromStagingTexture( uint8_t * data
		, ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		uint32_t size = getSize( extent, format );
		assert( size <= m_buffer.getSize() );
		auto buffer = m_buffer.lock( 0u
			, size
			, ashes::MemoryMapFlag::eWrite | ashes::MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw ashes::Exception{ ashes::Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( data
			, buffer
			, size );
		m_buffer.flush( 0u, size );
		m_buffer.unlock();
		m_device.waitIdle();
	}
}
