#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	BufferView::BufferView( VkDevice device
		, VkBufferViewCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_buffer{ createInfo.buffer }
		, m_format{ createInfo.format }
		, m_offset{ createInfo.offset }
		, m_range{ createInfo.range }
	{
		auto context = get( m_device )->getContext();
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
			, getInternalFormat( m_format )
			, get( createInfo.buffer )->getInternal()
			, get( createInfo.buffer )->getInternalOffset() + m_offset
			, m_range );
		glLogCall( context
			, glBindTexture
			, GL_BUFFER_TARGET_TEXTURE
			, 0u );
	}

	BufferView::~BufferView()
	{
		auto context = get( m_device )->getContext();
		glLogCall( context
			, glDeleteTextures
			, 1
			, &m_name );
	}
}
