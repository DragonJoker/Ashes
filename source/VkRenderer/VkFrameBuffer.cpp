/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkFrameBuffer.hpp"

#include "VkQueue.hpp"
#include "VkRenderPass.hpp"
#include "VkTexture.hpp"

#include <VkLib/Image.hpp>
#include <VkLib/ImageView.hpp>

namespace vk_renderer
{
	namespace
	{
		vk::ImageViewCRefArray convert( renderer::TextureCRefArray const & textures )
		{
			vk::ImageViewCRefArray result;
			result.reserve( textures.size() );

			for ( auto & texture : textures )
			{
				result.emplace_back( static_cast< Texture const & >( texture.get() ).getImage().getView() );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( renderer::RenderPass const & renderPass
		, utils::IVec2 const & dimensions
		, renderer::TextureCRefArray const & textures )
		: renderer::FrameBuffer{ renderPass, dimensions, textures }
		, m_frameBuffer{ static_cast< RenderPass const & >( renderPass ).getRenderPass().createCompatibleFrameBuffer( dimensions.x
			, dimensions.y
			, convert( textures ) ) }
	{
	}

	FrameBuffer::FrameBuffer( vk::FrameBufferPtr && frameBuffer )
		: renderer::FrameBuffer{}
		, m_frameBuffer{ std::move( frameBuffer ) }
	{
	}

	void FrameBuffer::download( renderer::Queue const & queue
		, uint32_t index
		, uint32_t xoffset
		, uint32_t yoffset
		, uint32_t width
		, uint32_t height
		, utils::PixelFormat format
		, uint8_t * data )const noexcept
	{
		m_frameBuffer->download( static_cast< Queue const & >( queue ).getQueue()
			, index
			, xoffset
			, yoffset
			, width
			, height
			, convert( format )
			, data );
	}
}
