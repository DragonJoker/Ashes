#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	BufferView::BufferView( Device const & device
		, Buffer const & buffer
		, VkBufferViewCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ createInfo }
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glGenTextures
			, 1
			, &m_name );
		glLogCall( context
			, glActiveTexture
			, GL_TEXTURE0 );
		glLogCall( context
			, glBindTexture
			, GL_BUFFER_TARGET_TEXTURE
			, m_name );
		glLogCall( context
			, glTexBufferRange
			, GL_BUFFER_TARGET_TEXTURE
			, getInternalFormat( m_createInfo.format )
			, buffer.getInternal()
			, m_createInfo.offset
			, m_createInfo.range );
		glLogCall( context
			, glBindTexture
			, GL_BUFFER_TARGET_TEXTURE
			, 0u );
	}

	BufferView::~BufferView()
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteTextures
			, 1
			, &m_name );
	}
}
