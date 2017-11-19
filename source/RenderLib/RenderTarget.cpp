#include "RenderTarget.h"

#include "Scene.h"

#include <GlLib/GlFrameBuffer.h>
#include <GlLib/GlRenderBuffer.h>
#include <GlLib/GlTexture.h>

namespace render
{
	RenderTarget::RenderTarget( gl::IVec2 const & dimensions
		, gl::PixelFormat format )
		: m_size{ dimensions }
		, m_framebuffer{ std::make_unique< gl::FrameBuffer >() }
		, m_colour{ std::make_unique< gl::Texture >( format, dimensions ) }
		, m_depth{ std::make_unique< gl::RenderBuffer >( gl::PixelFormat::eD16, dimensions ) }
	{
		m_framebuffer->bind();
		m_framebuffer->attach( *m_colour, gl::AttachmentPoint::eColour0 );
		m_framebuffer->attach( *m_depth, gl::AttachmentPoint::eDepth );
		m_framebuffer->unbind();
	}

	RenderTarget::~RenderTarget()noexcept
	{
		m_framebuffer->bind();
		m_framebuffer->detach( *m_colour, gl::AttachmentPoint::eColour0 );
		m_framebuffer->detach( *m_depth, gl::AttachmentPoint::eDepth );
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
