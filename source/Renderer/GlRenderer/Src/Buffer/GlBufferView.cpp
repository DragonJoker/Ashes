#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	BufferView::BufferView( Device const & device
		, Buffer const & buffer
		, renderer::Format format
		, uint32_t offset
		, uint32_t range )
		: renderer::BufferView{ device, buffer, format, offset, range }
		, m_device{ device }
	{
		glLogCall( m_device.getContext(), glGenTextures, 1, &m_name );
		glLogCall( m_device.getContext(), glActiveTexture, GL_TEXTURE0 );
		glLogCall( m_device.getContext(), glBindTexture, GL_BUFFER_TARGET_TEXTURE, m_name );
		glLogCall( m_device.getContext(), glTexBufferRange, GL_BUFFER_TARGET_TEXTURE, getInternal( format ), buffer.getBuffer(), offset, range );
		glLogCall( m_device.getContext(), glBindTexture, GL_BUFFER_TARGET_TEXTURE, 0u );
	}

	BufferView::~BufferView()
	{
		glLogCall( m_device.getContext(), glDeleteTextures, 1, &m_name );
	}
}
