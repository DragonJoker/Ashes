/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___D3D11Renderer_StagingBuffer_HPP___
#define ___D3D11Renderer_StagingBuffer_HPP___
#pragma once

#include "Image/D3D11Texture.hpp"

#include <Image/StagingTexture.hpp>

namespace d3d11_renderer
{
	class StagingTexture
		: public ashes::StagingTexture
	{
	public:
		StagingTexture( Device const & device
			, ashes::Format format
			, ashes::Extent2D const & extent );

		uint8_t * lock( ashes::Extent2D const & size
			, ashes::MemoryMapFlags flags )const override;
		void invalidate( ashes::Extent2D const & size )const override;
		void flush( ashes::Extent2D const & size )const override;
		void unlock()const override;

	private:
		void doCopyToStagingTexture( uint8_t const * const data
			, ashes::Format format
			, ashes::Extent2D const & extent )const override;
		void doCopyStagingToDestination( ashes::CommandBuffer const & commandBuffer
			, ashes::ImageSubresourceLayers const & subresourceLayers
			, ashes::Format format
			, ashes::Offset3D const & offset
			, ashes::Extent2D const & extent
			, ashes::TextureView const & texture )const override;

		void doCopyDestinationToStaging( ashes::CommandBuffer const & commandBuffer
			, ashes::ImageSubresourceLayers const & subresourceLayers
			, ashes::Format format
			, ashes::Offset3D const & offset
			, ashes::Extent2D const & extent
			, ashes::TextureView const & texture )const override;
		void doCopyFromStagingTexture( uint8_t * data
			, ashes::Format format
			, ashes::Extent2D const & extent )const override;

	protected:
		Device const & m_device;
		Texture m_texture;
		ashes::DeviceMemoryPtr m_storage;
	};
}

#endif
