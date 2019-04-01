/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Buffer/StagingBuffer.hpp"

#include "AshesRenderer/Buffer/Buffer.hpp"
#include "AshesRenderer/Command/CommandBuffer.hpp"
#include "AshesRenderer/Core/Device.hpp"
#include "AshesRenderer/Core/Exception.hpp"
#include "AshesRenderer/Image/Image.hpp"
#include "AshesRenderer/Image/ImageView.hpp"
#include "AshesRenderer/Image/ImageSubresourceLayers.hpp"
#include "AshesRenderer/Image/ImageSubresource.hpp"
#include "AshesRenderer/Image/SubresourceLayout.hpp"
#include "AshesRenderer/Sync/BufferMemoryBarrier.hpp"
#include "AshesRenderer/Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
	namespace
	{
		uint32_t deduceMemoryType( uint32_t typeBits
			, ashes::MemoryPropertyFlags requirements
			, ashes::PhysicalDeviceMemoryProperties const & memoryProperties )
		{
			uint32_t result = 0xFFFFFFFFu;
			bool found{ false };

			// Recherche parmi les types de m�moire la premi�re ayant les propri�t�s voulues.
			uint32_t i{ 0 };

			while ( i < memoryProperties.memoryTypes.size() && !found )
			{
				if ( ( typeBits & 1 ) == 1 )
				{
					// Le type de m�moire est disponible, a-t-il les propri�t�s demand�es?
					if ( ( memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
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
				throw ashes::Exception{ ashes::Result::eErrorRenderer, "Could not deduce memory type" };
			}

			return result;
		}

		template< typename T, typename U >
		T getAligned( T value, U align )
		{
			return T( ( value + align - 1 ) & ~( align - 1 ) );
		};

		template< typename T >
		T getSubresourceValue( T value, uint32_t mipLevel )
		{
			return T( value >> mipLevel );
		};

		ashes::MemoryAllocateInfo getAllocateInfo( Device const & device
			, BufferBase const & buffer )
		{
			auto requirements = buffer.getMemoryRequirements();
			auto deduced = deduceMemoryType( requirements.memoryTypeBits
				, ashes::MemoryPropertyFlag::eHostVisible
				, device.getMemoryProperties() );
			return { requirements.size, deduced };
		}

		ashes::Extent3D getTexelBlockExtent( ashes::Format format )
		{
			ashes::Extent3D texelBlockExtent{ 1u, 1u, 1u };

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

		uint32_t getTexelBlockByteSize( Extent3D const & texelBlockExtent
			, Format format )
		{
			uint32_t texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return texelBlockSize;
		}

		ashes::BufferImageCopy makeValidCopyInfo( Device const & device
			, Image const & image
			, Extent2D const & size
			, Offset3D offset
			, ImageSubresourceLayers const & subresourceLayers )
		{
			auto format = image.getFormat();
			auto type = image.getType();
			auto texelBlockExtent = getTexelBlockExtent( format );
			uint32_t texelBlockSize = getTexelBlockByteSize( texelBlockExtent, format );

			ImageSubresource subresource
			{
				subresourceLayers.aspectMask,
				subresourceLayers.mipLevel,
				subresourceLayers.baseArrayLayer,
			};
			SubresourceLayout layout;

			if ( image.getTiling() == ashes::ImageTiling::eLinear )
			{
				device.getImageSubresourceLayout( image
					, subresource
					, layout );
			}
			else
			{
				auto mipWidth = getSubresourceValue( image.getDimensions().width, subresource.mipLevel );
				auto mipHeight = getSubresourceValue( image.getDimensions().height, subresource.mipLevel );
				layout.rowPitch = texelBlockSize * mipWidth / ( texelBlockExtent.width * texelBlockExtent.height * texelBlockExtent.depth );
				layout.arrayPitch = layout.rowPitch * mipHeight * texelBlockExtent.height / ( texelBlockExtent.width * texelBlockExtent.depth );
				layout.depthPitch = layout.arrayPitch;
				layout.offset = subresource.arrayLayer * layout.arrayPitch * texelBlockSize;
				layout.size = layout.arrayPitch * image.getDimensions().depth;
			}

			ImageSubresourceLayers layers
			{
				subresourceLayers.aspectMask,
				subresourceLayers.mipLevel,
				subresourceLayers.baseArrayLayer,
				subresourceLayers.layerCount
			};
			layers.baseArrayLayer = type == ashes::ImageType::e3D
				? 0
				: layers.baseArrayLayer;
			layers.layerCount = type == ashes::ImageType::e3D
				? 1u
				: layers.layerCount;

			ashes::Extent3D imageExtent
			{
				size.width,
				size.height,
				1u,
			};
			imageExtent.width = getAligned( imageExtent.width, texelBlockExtent.width );
			imageExtent.height = getAligned( imageExtent.height, texelBlockExtent.height );
			imageExtent.depth = getAligned( imageExtent.depth, texelBlockExtent.depth );
			imageExtent.height = type == ashes::ImageType::e1D
				? 1u
				: imageExtent.height;
			imageExtent.depth = ( type == ashes::ImageType::e1D || type == ashes::ImageType::e2D )
				? 1u
				: imageExtent.depth;

			ashes::Extent3D subresourceExtent
			{
				getSubresourceValue( image.getDimensions().width, subresourceLayers.mipLevel ),
				getSubresourceValue( image.getDimensions().height, subresourceLayers.mipLevel ),
				image.getDimensions().depth,
			};
			subresourceExtent.width = getAligned( subresourceExtent.width, texelBlockExtent.width );
			subresourceExtent.height = getAligned( subresourceExtent.height, texelBlockExtent.height );
			subresourceExtent.depth = getAligned( subresourceExtent.depth, texelBlockExtent.depth );
			subresourceExtent.height = type == ashes::ImageType::e1D
				? 1u
				: subresourceExtent.height;
			subresourceExtent.depth = ( type == ashes::ImageType::e1D || type == ashes::ImageType::e2D )
				? 1u
				: subresourceExtent.depth;

			ashes::Offset3D imageOffset
			{
				std::min( int32_t( subresourceExtent.width - imageExtent.width ), std::max( 0, offset.x ) ),
				std::min( int32_t( subresourceExtent.height - imageExtent.height ), std::max( 0, offset.y ) ),
				std::min( int32_t( subresourceExtent.depth - imageExtent.depth ), std::max( 0, offset.z ) ),
			};

			offset.y = type == ashes::ImageType::e1D
				? 0
				: offset.y;
			offset.z = ( type == ashes::ImageType::e1D || type == ashes::ImageType::e2D )
				? 0
				: offset.z;

			auto bufferOffset = 0u;
			auto bufferRowLength = imageExtent.width;
			auto bufferImageHeight = imageExtent.height;

			return ashes::BufferImageCopy
			{
				bufferOffset,
				bufferRowLength,
				bufferImageHeight,
				layers,
				imageOffset,
				imageExtent,
				0u,
			};
		}

		ashes::BufferImageCopyArray makeValidCopyInfos( Device const & device
			, ImageView const & view
			, ImageSubresourceLayers subresourceLayers
			, Extent2D const & size
			, Offset3D const & offset )
		{
			ashes::BufferImageCopyArray copyInfos;
			copyInfos.push_back( makeValidCopyInfo( device
				, view.getImage()
				, size
				, offset
				, subresourceLayers ) );
			return copyInfos;
		}

		uint32_t getLevelSize( Extent2D extent
			, uint32_t level
			, Extent3D texelBlockExtent
			, uint32_t texelBlockSize )
		{
			extent.width >>= level;
			extent.height >>= level;
			return ( texelBlockSize
				* extent.width
				* extent.height ) / ( texelBlockExtent.width * texelBlockExtent.height );
		}

		uint32_t getLevelSize( Extent2D const & extent
			, Format format
			, uint32_t level )
		{
			auto texelBlockExtent = getTexelBlockExtent( format );
			return getLevelSize( extent
				, level
				, texelBlockExtent
				, getTexelBlockByteSize( texelBlockExtent, format ) );
		}

		uint32_t getAllLevelsSize( Extent2D const & extent
			, ashes::Format format
			, uint32_t baseMipLevel
			, uint32_t levelCount )
		{
			auto texelBlockExtent = getTexelBlockExtent( format );
			auto texelBlockSize = getTexelBlockByteSize( texelBlockExtent, format );
			uint32_t result = 0u;

			for ( uint32_t level = baseMipLevel; level < baseMipLevel + levelCount; ++level )
			{
				result += getLevelSize( extent
					, level
					, texelBlockExtent
					, texelBlockSize );
			}

			return result;
		}
	}

	StagingBuffer::StagingBuffer( Device const & device
		, BufferTargets target
		, uint32_t size )
		: m_device{ device }
		, m_buffer{ device.createBuffer( size
			, target | BufferTarget::eTransferSrc | BufferTarget::eTransferDst ) }
		, m_storage{ device.allocateMemory( getAllocateInfo( device, *m_buffer ) ) }
	{
		m_buffer->bindMemory( m_storage );
	}


	void StagingBuffer::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, uint8_t const * const data
		, ImageView const & view
		, PipelineStageFlags dstStageFlags )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data
			, view );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, uint8_t const * const data
		, ImageView const & view
		, PipelineStageFlags dstStageFlags )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadTextureData( *commandBuffer
			, format
			, data
			, view );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, uint8_t const * const data
		, ImageView const & view
		, PipelineStageFlags dstStageFlags )const
	{
		auto range = view.getSubResourceRange();
		auto layerSize = getAllLevelsSize( extent
			, format
			, range.baseMipLevel
			, range.levelCount );
		doCopyToStagingBuffer( data
			, layerSize );
		doCopyFromStagingBuffer( commandBuffer
			, extent
			, offset
			, view
			, dstStageFlags
			, subresourceLayers );
	}

	void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, Format format
		, uint8_t const * const data
		, ImageView const & view
		, PipelineStageFlags dstStageFlags )const
	{
		auto extent = Extent3D{ view.getImage().getDimensions() };
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		uploadTextureData( commandBuffer
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, format
			, Offset3D{}
			, { extent.width, extent.height }
			, data
			, view
			, dstStageFlags );
	}

	void StagingBuffer::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, uint8_t * data
		, ImageView const & view
		, PipelineStageFlags dstStageFlags )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eTopOfPipe
			, ashes::PipelineStageFlag::eTransfer
			, view.makeTransferSource( ashes::ImageLayout::eUndefined
				, 0u ) );
		doCopyToStagingBuffer( *commandBuffer
			, extent
			, offset
			, view
			, dstStageFlags
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			} );
		commandBuffer->memoryBarrier( ashes::PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eFragmentShader
			, view.makeShaderInputResource( ashes::ImageLayout::eTransferSrcOptimal
				, ashes::AccessFlag::eTransferRead ) );
		commandBuffer->end();

		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( ashes::FenceTimeout );

		auto range = view.getSubResourceRange();
		doCopyFromStagingBuffer( data
			, getAllLevelsSize( extent
				, format
				, range.baseMipLevel
				, range.levelCount ) );
	}

	void StagingBuffer::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, uint8_t * data
		, ImageView const & view
		, PipelineStageFlags dstStageFlags )const
	{
		auto extent = view.getImage().getDimensions();
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		downloadTextureData( queue
			, commandPool
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, format
			, Offset3D{}
			, { extent.width, extent.height }
			, data
			, view
			, dstStageFlags );
	}

	void StagingBuffer::doCopyFromStagingBuffer( uint8_t * data
		, uint32_t size )const
	{
		assert( size <= getBuffer().getSize() );
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
		mappedSize = mappedSize > getBuffer().getSize()
			? ~( 0ull )
			: mappedSize;
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).lock( 0u
			, mappedSize
			, MemoryMapFlag::eRead );

		if ( !buffer )
		{
			throw Exception{ Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( data
			, buffer
			, size );
		getBuffer().flush( 0u, mappedSize );
		getBuffer().unlock();
		m_device.waitIdle();
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferSource() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer
			, size
			, offset );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eVertexInput
			, buffer.getBuffer().makeVertexShaderInputResource() );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, dstStageFlags
			, buffer.getBuffer().makeUniformBufferInput() );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, Extent2D const & size
		, Offset3D const & offset
		, ImageView const & view
		, PipelineStageFlags dstStageFlags
		, ImageSubresourceLayers const & subresourceLayers )const
	{
		auto realSize = getAllLevelsSize( size
			, view.getFormat()
			, view.getSubResourceRange().baseMipLevel
			, view.getSubResourceRange().levelCount );
		assert( realSize <= getBuffer().getSize() );

		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferSource() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eBottomOfPipe
			, PipelineStageFlag::eTransfer
			, view.makeTransferDestination( ashes::ImageLayout::eUndefined, 0u ) );
		commandBuffer.copyToImage( makeValidCopyInfos( m_device, view, subresourceLayers, size, offset )
			, getBuffer()
			, view.getImage() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, dstStageFlags
			, view.makeShaderInputResource( ashes::ImageLayout::eTransferDstOptimal
				, ashes::AccessFlag::eTransferWrite ) );
	}

	void StagingBuffer::doCopyToStagingBuffer( uint8_t const * data
		, uint32_t size )const
	{
		assert( size <= getBuffer().getSize() );
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
		mappedSize = mappedSize > getBuffer().getSize()
			? ~( 0ull )
			: mappedSize;
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).lock( 0u
			, mappedSize
			, MemoryMapFlag::eWrite | MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw Exception{ Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size );
		getBuffer().flush( 0u, mappedSize );
		getBuffer().unlock();
		m_device.waitIdle();
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.makeTransferSource() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer
			, getBuffer()
			, size
			, offset );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, ashes::PipelineStageFlag::eVertexInput
			, buffer.getBuffer().makeVertexShaderInputResource() );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, dstStageFlags
			, buffer.getBuffer().makeUniformBufferInput() );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, Extent2D const & size
		, Offset3D const & offset
		, ImageView const & view
		, PipelineStageFlags dstStageFlags
		, ImageSubresourceLayers const & subresourceLayers )const
	{
		commandBuffer.memoryBarrier( PipelineStageFlag::eBottomOfPipe
			, PipelineStageFlag::eTransfer
			, view.makeTransferSource( ashes::ImageLayout::eUndefined, 0u ) );
		commandBuffer.memoryBarrier( PipelineStageFlag::eBottomOfPipe
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyToBuffer( makeValidCopyInfos( m_device, view, subresourceLayers, size, offset )
			, view.getImage()
			, getBuffer() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, dstStageFlags
			, view.makeShaderInputResource( ashes::ImageLayout::eTransferDstOptimal
				, ashes::AccessFlag::eTransferWrite ) );
	}
}
