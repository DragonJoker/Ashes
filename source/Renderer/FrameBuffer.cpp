/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "FrameBuffer.hpp"

#include "Queue.hpp"
#include "RenderPass.hpp"
#include "Texture.hpp"

#include <VkLib/Image.hpp>
#include <VkLib/ImageView.hpp>

namespace renderer
{
	namespace
	{
		vk::ImageViewCRefArray convert( TextureCRefArray const & textures )
		{
			vk::ImageViewCRefArray result;
			result.reserve( textures.size() );

			for ( auto & texture : textures )
			{
				result.emplace_back( texture.get().getImage().getView() );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, utils::IVec2 const & dimensions
		, TextureCRefArray const & textures )
		: m_frameBuffer{ renderPass.getRenderPass().createCompatibleFrameBuffer( dimensions.x
			, dimensions.y
			, convert( textures ) ) }
	{
	}

	FrameBuffer::FrameBuffer( vk::FrameBufferPtr && frameBuffer )
		: m_frameBuffer{ std::move( frameBuffer ) }
	{
	}

	void FrameBuffer::download( Queue const & queue
		, uint32_t index
		, uint32_t xoffset
		, uint32_t yoffset
		, uint32_t width
		, uint32_t height
		, utils::PixelFormat format
		, uint8_t * data )const noexcept
	{
		m_frameBuffer->download( queue.getQueue()
			, index
			, xoffset
			, yoffset
			, width
			, height
			, convert( format )
			, data );
	}
}
