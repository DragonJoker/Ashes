#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	BufferView::BufferView( renderer::Device const & device
		, Buffer const & buffer
		, renderer::PixelFormat format
		, uint32_t offset
		, uint32_t range )
		: renderer::BufferView{ device, buffer, format, offset, range }
	{
		glGenTextures( 1, &m_name );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_BUFFER, m_name );
		glTexBufferRange( GL_TEXTURE_BUFFER, getInternal( format ), buffer.getBuffer(), offset, range );
		glBindTexture( GL_TEXTURE_BUFFER, 0u );
	}

	BufferView::~BufferView()
	{
		glDeleteTextures( 1, &m_name );
	}
}
