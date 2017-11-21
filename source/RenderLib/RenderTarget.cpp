#include "RenderTarget.h"

#include "Scene.h"

#include <Renderer/GlFrameBuffer.h>
#include <Renderer/GlRenderBuffer.h>
#include <Renderer/GlTexture.h>

namespace render
{
	RenderTarget::RenderTarget( renderer::IVec2 const & dimensions
		, renderer::PixelFormat format )
		: m_size{ dimensions }
		, m_framebuffer{ std::make_unique< renderer::FrameBuffer >() }
		, m_colour{ std::make_unique< renderer::Texture >( format, dimensions ) }
		, m_depth{ std::make_unique< renderer::RenderBuffer >( renderer::PixelFormat::eD16, dimensions ) }
	{
		m_framebuffer->bind();
		m_framebuffer->attach( *m_colour, renderer::AttachmentPoint::eColour0 );
		m_framebuffer->attach( *m_depth, renderer::AttachmentPoint::eDepth );
		m_framebuffer->unbind();
	}

	RenderTarget::~RenderTarget()noexcept
	{
		m_framebuffer->bind();
		m_framebuffer->detach( *m_colour, renderer::AttachmentPoint::eColour0 );
		m_framebuffer->detach( *m_depth, renderer::AttachmentPoint::eDepth );
		m_framebuffer->unbind();
	}

	void RenderTarget::drawScene( Scene const & scene )const noexcept
	{
		m_framebuffer->bind();
		m_framebuffer->clear( scene.backgroundColour() );
		scene.draw();
		m_framebuffer->unbind();
	}
}
