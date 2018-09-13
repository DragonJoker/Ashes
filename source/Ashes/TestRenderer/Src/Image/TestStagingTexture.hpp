/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___TestRenderer_StagingBuffer_HPP___
#define ___TestRenderer_StagingBuffer_HPP___
#pragma once

#include "Buffer/TestBuffer.hpp"

#include <Image/StagingTexture.hpp>

namespace test_renderer
{
	class StagingTexture
		: public ashes::StagingTexture
	{
	public:
		StagingTexture( Device const & device
			, ashes::Format format
			, ashes::Extent3D const & extent );

	private:
		void doCopyToStagingTexture( uint8_t const * const data
			, ashes::Format format
			, ashes::Extent3D const & extent )const override;
		void doCopyStagingToDestination( ashes::CommandBuffer const & commandBuffer
			, ashes::ImageSubresourceLayers const & subresourceLayers
			, ashes::Format format
			, ashes::Offset3D const & offset
			, ashes::Extent3D const & extent
			, ashes::TextureView const & texture )const override;

		void doCopyDestinationToStaging( ashes::CommandBuffer const & commandBuffer
			, ashes::ImageSubresourceLayers const & subresourceLayers
			, ashes::Format format
			, ashes::Offset3D const & offset
			, ashes::Extent3D const & extent
			, ashes::TextureView const & texture )const override;
		void doCopyFromStagingTexture( uint8_t * data
			, ashes::Format format
			, ashes::Extent3D const & extent )const override;

	protected:
		Device const & m_device;
		Buffer m_buffer;
	};
}

#endif
