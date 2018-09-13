/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	/**
	*\name
	*	Upload.
	**/
	/**@{*/
	inline void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent3D const & extent
		, ByteArray const & data
		, TextureView const & texture )const
	{
		uploadTextureData( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture );
	}

	inline void StagingTexture::uploadTextureData( CommandBuffer const & commandBuffer
		, Format format
		, ByteArray const & data
		, TextureView const & texture )const
	{
		uploadTextureData( commandBuffer
			, format
			, data.data()
			, texture );
	}
	/**@}*/
	/**
	*\name
	*	Upload.
	**/
	/**@{*/
	inline void StagingTexture::downloadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent3D const & extent
		, ByteArray & data
		, TextureView const & texture )const
	{
		downloadTextureData( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture );
	}

	inline void StagingTexture::downloadTextureData( CommandBuffer const & commandBuffer
		, Format format
		, ByteArray & data
		, TextureView const & texture )const
	{
		downloadTextureData( commandBuffer
			, format
			, data.data()
			, texture );
	}
	/**@}*/
}
