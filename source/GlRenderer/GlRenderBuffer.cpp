#include "GlRenderBuffer.hpp"

#include "GlDevice.hpp"
#include "GlImageMemoryBarrier.hpp"

namespace gl_renderer
{
	RenderBuffer::RenderBuffer( renderer::Device const & device
		, utils::PixelFormat format
		, renderer::IVec2 const & size )
		: renderer::RenderBuffer{ device, format, size }
	{
		glCreateRenderbuffers( 1, &m_name );
		makeDepthStencilAttachment();
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteRenderbuffers( 1, &m_name );
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeColourAttachment()const
	{
		return renderer::ImageMemoryBarrier{ renderer::AccessFlag::eColourAttachmentWrite
			, renderer::AccessFlag::eColourAttachmentWrite
			, renderer::ImageLayout::eColourAttachmentOptimal
			, renderer::ImageLayout::eColourAttachmentOptimal
			, 0u
			, 0u
			, 
			, renderer::ImageSubresourceRange{  } };
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeDepthStencilAttachment()const
	{
		return renderer::ImageMemoryBarrier{ renderer::AccessFlag::eDepthStencilAttachmentWrite
			, renderer::AccessFlag::eDepthStencilAttachmentWrite
			, renderer::ImageLayout::eDepthStencilAttachmentOptimal
			, renderer::ImageLayout::eDepthStencilAttachmentOptimal
			, 0u
			, 0u
			,
			, renderer::ImageSubresourceRange{} };
	}
}
