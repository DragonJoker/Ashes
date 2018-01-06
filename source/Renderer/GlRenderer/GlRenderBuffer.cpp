#include "GlRenderBuffer.hpp"

#include "GlDevice.hpp"
#include "GlImageMemoryBarrier.hpp"

#include <Renderer/ImageSubresourceRange.hpp>

namespace gl_renderer
{
	namespace
	{
		renderer::ImageUsageFlag getAttachmentFlag( utils::PixelFormat format )
		{
			switch ( format )
			{
			case utils::PixelFormat::eL8:
			case utils::PixelFormat::eL8A8:
			case utils::PixelFormat::eR8G8B8:
			case utils::PixelFormat::eRGB565:
			case utils::PixelFormat::eR8G8B8A8:
			case utils::PixelFormat::eB8G8R8A8:
			case utils::PixelFormat::eRGBA5551:
			case utils::PixelFormat::eRGBA4444:
				return renderer::ImageUsageFlag::eColourAttachment;

			case utils::PixelFormat::eD16:
			case utils::PixelFormat::eD24S8:
			case utils::PixelFormat::eS8:
				return renderer::ImageUsageFlag::eDepthStencilAttachment;

			default:
				assert( false && "Unsupported pixel format." );
				return renderer::ImageUsageFlag::eColourAttachment;
			}
		}
	}

	RenderBuffer::RenderBuffer( renderer::Device const & device
		, utils::PixelFormat format
		, renderer::IVec2 const & size )
		: renderer::RenderBuffer{ device, format, size }
		, m_texture{ device }
	{
		m_texture.setImage( format
			, size
			, getAttachmentFlag( format )
			, renderer::ImageTiling::eOptimal );
		makeDepthStencilAttachment();
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeColourAttachment()const
	{
		return m_texture.makeColourAttachment();
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeDepthStencilAttachment()const
	{
		return m_texture.makeDepthStencilAttachment();
	}
}
