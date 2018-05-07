#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlRenderer.hpp"

#include <Miscellaneous/Log.hpp>

namespace gl_renderer
{
	BufferView::BufferView( Device const & device
		, Buffer const & buffer
		, renderer::Format format
		, uint32_t offset
		, uint32_t range )
		: renderer::BufferView{ device, buffer, format, offset, range }
	{
		glLogCall( gl::GenTextures, 1, &m_name );
		glLogCall( gl::ActiveTexture, GL_TEXTURE0 );
		glLogCall( gl::BindTexture, GL_BUFFER_TARGET_TEXTURE, m_name );

		if ( device.getRenderer().getFeatures().hasTexBufferRange )
		{
			glLogCall( gl::TexBufferRange_ARB, GL_BUFFER_TARGET_TEXTURE, getInternal( format ), buffer.getBuffer(), offset, range );
		}
		else
		{
			glLogCall( gl::TexBuffer, GL_BUFFER_TARGET_TEXTURE, getInternal( format ), buffer.getBuffer() );
		}

		glLogCall( gl::BindTexture, GL_BUFFER_TARGET_TEXTURE, 0u );
	}

	BufferView::~BufferView()
	{
		glLogCall( gl::DeleteTextures, 1, &m_name );
	}
}
