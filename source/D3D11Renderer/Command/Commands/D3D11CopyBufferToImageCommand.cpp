/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyBufferToImageCommand.hpp"
#include "Command/Commands/D3D11CopyImageCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"

#include <Ashes/Image/ImageSubresource.hpp>
#include <Ashes/Image/ImageSubresourceRange.hpp>
#include <Ashes/Image/SubresourceLayout.hpp>

namespace d3d11_renderer
{
	namespace
	{
		struct TexelCoordinates
		{
			ashes::Offset3D offset;
			uint32_t layer;
		};
		
		struct CompressedTexelBlockCoordinates
		{
			CompressedTexelBlockCoordinates( ashes::Offset3D const & offset
				, uint32_t const & layer
				, ashes::Extent2D const & texelBlockSize )
				: texel{ {
						int32_t( offset.x * texelBlockSize.width ),
						int32_t( offset.x * texelBlockSize.height ),
						offset.z,
					}, layer }
				, offset{
						int32_t( texel.offset.x / texelBlockSize.width ),
						int32_t( texel.offset.y / texelBlockSize.height ),
						texel.offset.z,
					}
				, layer{ texel.layer }
			{
			}
			
			CompressedTexelBlockCoordinates( TexelCoordinates const & coordinates
				, ashes::Extent2D const & texelBlockSize )
				: texel{ coordinates }
				, offset{
						int32_t( texel.offset.x / texelBlockSize.width ),
						int32_t( texel.offset.y / texelBlockSize.height ),
						texel.offset.z,
					}
				, layer{ texel.layer }
			{
			}

			TexelCoordinates texel;
			ashes::Offset3D offset;
			uint32_t layer;
		};

		uint64_t getSubresourceAddress( TexelCoordinates const & coordinates
			, ashes::SubresourceLayout const & layout
			, uint32_t elementSize
			, uint32_t offset )
		{
			return coordinates.layer * layout.arrayPitch
				+ coordinates.offset.z * layout.depthPitch
				+ coordinates.offset.y * layout.rowPitch
				+ coordinates.offset.x * elementSize + offset;
		}

		uint64_t getSubresourceAddress( CompressedTexelBlockCoordinates const & coordinates
			, ashes::SubresourceLayout const & layout
			, uint32_t compressedTexelBlockByteSize
			, uint32_t offset )
		{
			return coordinates.layer * layout.arrayPitch
				+ coordinates.offset.z * layout.depthPitch
				+ coordinates.offset.y * layout.rowPitch
				+ coordinates.offset.x * compressedTexelBlockByteSize + offset;
		}

		uint32_t getBufferRowPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return ( copyInfo.bufferRowLength
				? copyInfo.bufferRowLength
				: copyInfo.imageExtent.width );
		}
		
		uint32_t getBufferHeightPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return ( copyInfo.bufferImageHeight
				? copyInfo.bufferImageHeight
				: copyInfo.imageExtent.height );
		}

		uint32_t getBufferDepthPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return copyInfo.imageExtent.depth;
		}
		
		uint32_t getBufferLayerPitch( ashes::BufferImageCopy const & copyInfo )
		{
			return getBufferRowPitch( copyInfo )
				* getBufferHeightPitch( copyInfo );
		}

		std::vector< D3D11_BOX > doGetSrcBoxes( Image const & image
			, ashes::BufferImageCopyArray const & copyInfos )
		{
			std::vector< D3D11_BOX > result;

			for ( auto & copyInfo : copyInfos )
			{
				auto size = getSize( ashes::Extent3D
					{
						std::max( 1u, getBufferRowPitch( copyInfo ) ),
						std::max( 1u, getBufferHeightPitch( copyInfo ) ),
						std::max( 1u, getBufferDepthPitch( copyInfo ) )
					}
					, image.getFormat() );
				result.push_back(
					{
						UINT( copyInfo.bufferOffset ),
						0u,
						0u,
						UINT( copyInfo.bufferOffset + size ),
						1u,
						1u
					}
				);
			}

			return result;
		}

		std::vector< ashes::SubresourceLayout > doGetDstLayouts( Device const & device
			, Image const & image
			, ashes::BufferImageCopyArray const & copyInfos )
		{
			std::vector< ashes::SubresourceLayout > result;
			result.reserve( copyInfos.size() );

			for ( auto & copyInfo : copyInfos )
			{
				result.push_back( {} );
				device.getImageSubresourceLayout( image
					, ashes::ImageSubresource
					{
						copyInfo.imageSubresource.aspectMask,
						copyInfo.imageSubresource.mipLevel,
						copyInfo.imageSubresource.baseArrayLayer,
					}
					, result.back() );
			}

			return result;
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

		uint32_t getTexelBlockByteSize( ashes::Extent3D const & texelBlockExtent
			, ashes::Format format )
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

		void doCopyMapped( ashes::Format format
			, uint8_t const * srcBuffer
			, ashes::BufferImageCopy const & copyInfo
			, D3D11_BOX const & srcBox
			, uint8_t * dstBuffer
			, ashes::SubresourceLayout const & dstLayout )
		{
			auto extent = getTexelBlockExtent( format );
			auto extentCombined = extent.width * extent.height * extent.depth;
			auto byteSize = getTexelBlockByteSize( extent, format );
			auto bufferSize = srcBox.right - srcBox.left;
			auto bufferRowPitch = byteSize * getBufferRowPitch( copyInfo ) / extentCombined;
			//auto bufferHeightPitch = byteSize * getBufferHeightPitch( copyInfo ) / extentCombined;
			auto bufferHeightPitch = getBufferHeightPitch( copyInfo );
			auto bufferLayerPitch = bufferRowPitch * bufferHeightPitch;
			auto bufferDepthPitch = bufferLayerPitch * getBufferDepthPitch( copyInfo );
			auto imageSize = dstLayout.size;
			auto imageRowPitch = dstLayout.rowPitch;
			auto imageLayerPitch = dstLayout.arrayPitch;
			auto imageDepthPitch = dstLayout.depthPitch;
			auto imageWidth = copyInfo.imageExtent.width;
			auto imageHeight = copyInfo.imageExtent.height;
			auto imageDepth = copyInfo.imageExtent.depth;

			for ( auto l = 0u; l < copyInfo.imageSubresource.layerCount; ++l )
			{
				auto srcLayer = srcBuffer;
				auto dstLayer = dstBuffer;

				for ( auto z = 0u; z < imageDepth; ++z )
				{
					auto srcPlane = srcLayer;
					auto dstPlane = dstLayer;

					for ( auto y = 0u; y < imageHeight; ++y )
					{
						std::memcpy( dstPlane
							, srcPlane
							, std::min( uint64_t( bufferRowPitch ), imageRowPitch ) );
						srcPlane += bufferRowPitch;
						dstPlane += imageRowPitch;
					}

					srcLayer += bufferDepthPitch;
					dstLayer += imageDepthPitch;
				}

				srcBuffer += bufferLayerPitch;
				dstBuffer += imageLayerPitch;
			}
		}
	}

	ashes::ImagePtr getStagingTexture( Device const & device
		, ashes::Image const & image
		, ashes::Extent3D dimensions )
	{
		ashes::ImagePtr result = std::make_unique< Image >( device
			, ashes::ImageCreateInfo
			{
				0u,
				image.getType(),
				image.getFormat(),
				ashes::Extent3D
				{
					dimensions.width,
					dimensions.height,
					dimensions.depth,
				},
				1u,
				image.getLayerCount(),
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eLinear,
				ashes::ImageUsageFlag::eTransferSrc | ashes::ImageUsageFlag::eTransferDst,
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined,
			} );
		auto requirements = result->getMemoryRequirements();
		uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
			, ashes::MemoryPropertyFlag::eHostVisible );
		result->bindMemory( device.allocateMemory( { requirements.size, deduced } ) );
		return result;
	}

	CopyBufferToImageCommand::CopyBufferToImageCommand( Device const & device
		, ashes::BufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Image const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Image const & >( dst ) }
		, m_format{ getSRVFormat( m_dst.getFormat() ) }
		, m_srcBoxes{ doGetSrcBoxes( m_dst, copyInfo ) }
		, m_dstLayouts{ doGetDstLayouts( device, m_dst, copyInfo ) }
		, m_mappable{ m_dst.getMemoryRequirements().memoryTypeBits == ashes::MemoryPropertyFlag::eHostVisible }
	{
	}

	void CopyBufferToImageCommand::apply( Context const & context )const
	{
		if ( m_mappable )
		{
			for ( auto i = 0u; i < m_copyInfo.size(); ++i )
			{
				applyOne( context
					, m_copyInfo[i]
					, m_srcBoxes[i]
					, m_dstLayouts[i]
					, m_dst );
			}
		}
		else
		{
			if ( !m_copyInfo.empty() )
			{
				for ( auto i = 0u; i < m_copyInfo.size(); ++i )
				{
					applyOneStaging( context
						, m_copyInfo[i]
						, m_srcBoxes[i]
						, m_dstLayouts[i] );
				}
			}
		}
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}

	void CopyBufferToImageCommand::applyOneStaging( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, D3D11_BOX const & srcBox
		, ashes::SubresourceLayout const & dstLayout )const
	{
		auto staging = getStagingTexture( m_device
			, m_dst
			, copyInfo.imageExtent );
		applyOne( context
			, copyInfo
			, srcBox
			, dstLayout
			, *staging );
		ashes::ImageSubresourceLayers stagingSurbresouce{ copyInfo.imageSubresource };
		stagingSurbresouce.mipLevel = 0u;
		stagingSurbresouce.baseArrayLayer = 0u;
		CopyImageCommand command{ m_device
			, ashes::ImageCopy
			{
				stagingSurbresouce,
				ashes::Offset3D{},
				copyInfo.imageSubresource,
				copyInfo.imageOffset,
				ashes::Extent3D
				{
					copyInfo.imageExtent.width,
					copyInfo.imageExtent.height,
					copyInfo.imageExtent.depth,
				},
			}
			, *staging
			, m_dst };
		command.apply( context );
	}

	void CopyBufferToImageCommand::applyOne( Context const & context
		, ashes::BufferImageCopy const & copyInfo
		, D3D11_BOX const & srcBox
		, ashes::SubresourceLayout const & dstLayout
		, ashes::Image const & image )const
	{
		auto bufferSize = srcBox.right - srcBox.left;
		auto imageSize = dstLayout.size;

		if ( auto srcBuffer = m_src.lock( srcBox.left
			, bufferSize
			, ashes::MemoryMapFlag::eRead ) )
		{
			if ( auto dstBuffer = image.lock( dstLayout
				, ashes::MemoryMapFlag::eWrite ) )
			{
				doCopyMapped( image.getFormat()
					, srcBuffer
					, copyInfo
					, srcBox
					, dstBuffer
					, dstLayout );
				image.flush( dstLayout );
				image.unlock();
			}

			m_src.flush( srcBox.left
				, bufferSize );
			m_src.unlock();
		}
	}
}
