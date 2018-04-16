/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/CommandBuffer.hpp"

#include "Buffer/VertexBuffer.hpp"
#include "Buffer/UniformBuffer.hpp"
#include "Image/ImageSubresourceRange.hpp"
#include "Image/Texture.hpp"
#include "Image/TextureView.hpp"
#include "Sync/BufferMemoryBarrier.hpp"
#include "Sync/ImageMemoryBarrier.hpp"

namespace renderer
{
	namespace
	{
		bool areCompatible( PipelineStageFlags pipelineFlags
			, AccessFlags accessFlags )
		{
			if ( checkFlag( pipelineFlags, PipelineStageFlag::eAllCommands )
				|| checkFlag( pipelineFlags, PipelineStageFlag::eAllGraphics ) )
			{
				return true;
			}

			if ( checkFlag( accessFlags, AccessFlag::eIndirectCommandRead ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eDrawIndirect );
			}

			if ( checkFlag( accessFlags, AccessFlag::eIndexRead )
				|| checkFlag( accessFlags, AccessFlag::eVertexAttributeRead ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eVertexInput );
			}

			if ( checkFlag( accessFlags, AccessFlag::eInputAttachmentRead ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eFragmentShader );
			}

			if ( checkFlag( accessFlags, AccessFlag::eShaderRead )
				|| checkFlag( accessFlags, AccessFlag::eShaderWrite )
				|| checkFlag( accessFlags, AccessFlag::eUniformRead ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eVertexShader )
					|| checkFlag( pipelineFlags, PipelineStageFlag::eTessellationControlShader )
					|| checkFlag( pipelineFlags, PipelineStageFlag::eTessellationEvaluationShader )
					|| checkFlag( pipelineFlags, PipelineStageFlag::eGeometryShader )
					|| checkFlag( pipelineFlags, PipelineStageFlag::eFragmentShader )
					|| checkFlag( pipelineFlags, PipelineStageFlag::eComputeShader );
			}

			if ( checkFlag( accessFlags, AccessFlag::eColourAttachmentRead )
				|| checkFlag( accessFlags, AccessFlag::eColourAttachmentWrite ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eColourAttachmentOutput );
			}

			if ( checkFlag( accessFlags, AccessFlag::eDepthStencilAttachmentRead )
				|| checkFlag( accessFlags, AccessFlag::eDepthStencilAttachmentWrite ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eEarlyFragmentTests )
					|| checkFlag( pipelineFlags, PipelineStageFlag::eLateFragmentTests );
			}

			if ( checkFlag( accessFlags, AccessFlag::eTransferRead )
				|| checkFlag( accessFlags, AccessFlag::eTransferWrite ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eTransfer );
			}

			if ( checkFlag( accessFlags, AccessFlag::eHostRead )
				|| checkFlag( accessFlags, AccessFlag::eHostWrite ) )
			{
				return checkFlag( pipelineFlags, PipelineStageFlag::eHost );
			}

			return true;
		}
	}

	CommandBuffer::CommandBuffer( Device const & device
		, CommandPool const & pool
		, bool primary )
		: m_device{ device }
	{
		registerObject( m_device, "CommandBuffer", this );
	}

	CommandBuffer::~CommandBuffer()
	{
		unregisterObject( m_device, this );
	}

	void CommandBuffer::bindVertexBuffer( uint32_t binding
		, BufferBase const & buffer
		, uint64_t offset )const
	{
		bindVertexBuffers( binding
			, BufferCRefArray{ buffer }
			, UInt64Array{ offset } );
	}

	void CommandBuffer::copyToImage( BufferImageCopy const & copyInfo
		, BufferBase const & src
		, Texture const & dst )const
	{
		copyToImage( BufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyToBuffer( BufferImageCopy const & copyInfo
		, Texture const & src
		, BufferBase const & dst )const
	{
		copyToBuffer( BufferImageCopyArray{ 1u, copyInfo }
			, src
			, dst );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src, dst );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src, dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( VertexBufferBase const & src
		, BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst );
	}

	void CommandBuffer::copyBuffer( VertexBufferBase const & src
		, VertexBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( BufferBase const & src
		, UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src, dst.getBuffer() );
	}

	void CommandBuffer::copyBuffer( UniformBufferBase const & src
		, BufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst );
	}

	void CommandBuffer::copyBuffer( UniformBufferBase const & src
		, UniformBufferBase const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		BufferCopy copyInfo
		{
			offset,
			0,
			size
		};
		copyBuffer( copyInfo, src.getBuffer(), dst.getBuffer() );
	}

	void CommandBuffer::copyImage( TextureView const & src
		, TextureView const & dst )const
	{
		auto const & srcRange = src.getSubResourceRange();
		auto const & dstRange = dst.getSubResourceRange();
		copyImage( ImageCopy
			{
				{                                                   // srcSubresource
					getAspectMask( src.getFormat() ),
					srcRange.baseMipLevel,
					srcRange.baseArrayLayer,
					srcRange.layerCount
				},
				Offset3D{                                              // srcOffset
					0,                                                  // x
					0,                                                  // y
					0                                                   // z
				},
				{                                                   // dstSubresource
					getAspectMask( dst.getFormat() ),
					dstRange.baseMipLevel,
					dstRange.baseArrayLayer,
					dstRange.layerCount
				},
				Offset3D{                                              // dstOffset
					0,                                                  // x
					0,                                                  // y
					0                                                   // z
				},
				dst.getTexture().getDimensions()                    // extent
			}
			, src.getTexture()
			, ImageLayout::eTransferSrcOptimal
			, dst.getTexture()
			, ImageLayout::eTransferDstOptimal );
	}

	void CommandBuffer::memoryBarrier( PipelineStageFlags after
		, PipelineStageFlags before
		, BufferMemoryBarrier const & transitionBarrier )const
	{
		assert( areCompatible( after, transitionBarrier.getSrcAccessMask() ) );
		assert( areCompatible( before, transitionBarrier.getDstAccessMask() ) );
		doMemoryBarrier( after, before, transitionBarrier );
	}

	void CommandBuffer::memoryBarrier( PipelineStageFlags after
		, PipelineStageFlags before
		, ImageMemoryBarrier const & transitionBarrier )const
	{
		assert( areCompatible( after, transitionBarrier.getSrcAccessMask() ) );
		assert( areCompatible( before, transitionBarrier.getDstAccessMask() ) );
		doMemoryBarrier( after, before, transitionBarrier );
	}
}
