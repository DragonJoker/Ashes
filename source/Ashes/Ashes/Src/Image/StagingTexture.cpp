/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Image/StagingTexture.hpp"

#include "Core/Device.hpp"
#include "Core/Exception.hpp"
#include "Image/Texture.hpp"
#include "Image/TextureView.hpp"
#include "Sync/BufferMemoryBarrier.hpp"
#include "Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
	StagingTexture::StagingTexture( Device const & device
		, Extent3D const & extent )
		: m_device{ device }
	{
	}

	void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent3D const & extent
		, uint8_t const * const data
		, TextureView const & view )const
	{
		doCopyToStagingTexture( data
			, format
			, extent );
		doCopyStagingToDestination( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
	}

	void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, Format format
		, uint8_t const * const data
		, TextureView const & view )const
	{
		auto extent = Extent3D{ view.getTexture().getDimensions() };
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		extent.depth = std::max( 1u, extent.depth >> mipLevel );
		uploadTextureData( commandBuffer
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, format
			, Offset3D{ 0, 0, 0 }
			, extent
			, data
			, view );
	}

	void StagingTexture::downloadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent3D const & extent
		, uint8_t * data
		, TextureView const & view )const
	{
		doCopyDestinationToStaging( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, view );
		doCopyFromStagingTexture( data
			, format
			, extent );
	}

	void StagingTexture::downloadTextureData( CommandBuffer const & commandBuffer
		, Format format
		, uint8_t * data
		, TextureView const & view )const
	{
		auto extent = Extent3D{ view.getTexture().getDimensions() };
		auto mipLevel = view.getSubResourceRange().baseMipLevel;
		extent.width = std::max( 1u, extent.width >> mipLevel );
		extent.height = std::max( 1u, extent.height >> mipLevel );
		extent.depth = std::max( 1u, extent.depth >> mipLevel );
		downloadTextureData( commandBuffer
			, {
				getAspectMask( view.getFormat() ),
				mipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, format
			, Offset3D{ 0, 0, 0 }
			, extent
			, data
			, view );
	}
}
