/*
This file belongs to RendererLib.
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
		: public renderer::StagingTexture
	{
	public:
		StagingTexture( Device const & device
			, renderer::Format format
			, renderer::Extent3D const & extent );

	private:
		void doCopyToStagingTexture( uint8_t const * const data
			, renderer::Format format
			, renderer::Extent3D const & extent )const override;
		void doCopyStagingToDestination( renderer::CommandBuffer const & commandBuffer
			, renderer::ImageSubresourceLayers const & subresourceLayers
			, renderer::Format format
			, renderer::Offset3D const & offset
			, renderer::Extent3D const & extent
			, renderer::TextureView const & texture )const override;

		void doCopyDestinationToStaging( renderer::CommandBuffer const & commandBuffer
			, renderer::ImageSubresourceLayers const & subresourceLayers
			, renderer::Format format
			, renderer::Offset3D const & offset
			, renderer::Extent3D const & extent
			, renderer::TextureView const & texture )const override;
		void doCopyFromStagingTexture( uint8_t * data
			, renderer::Format format
			, renderer::Extent3D const & extent )const override;

	protected:
		Device const & m_device;
		Texture m_texture;
	};
}

#endif
