/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlFrameBuffer.hpp"

#include "GlQueue.hpp"
#include "GlRenderPass.hpp"
#include "GlTexture.hpp"

namespace gl_renderer
{
	FrameBuffer::FrameBuffer( renderer::RenderPass const & renderPass
		, utils::IVec2 const & dimensions
		, renderer::TextureCRefArray const & textures )
		: renderer::FrameBuffer{ renderPass, dimensions, textures }
	{
		glGenFramebuffers( 1, &m_frameBuffer );
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers( 1, &m_frameBuffer );
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
		//m_frameBuffer->download( static_cast< Queue const & >( queue ).getQueue()
		//	, index
		//	, xoffset
		//	, yoffset
		//	, width
		//	, height
		//	, convert( format )
		//	, data );
	}
}
