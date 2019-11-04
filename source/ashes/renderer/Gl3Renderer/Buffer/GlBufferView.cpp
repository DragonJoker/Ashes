#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	BufferView::BufferView( VkDevice device
		, VkBufferViewCreateInfo createInfo )
		: m_device{ device }
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

		if ( get( get( device )->getInstance() )->getFeatures().hasBufferRange )
		{
			glLogCall( context
				, glTexBufferRange_ARB
				, GL_BUFFER_TARGET_TEXTURE
				, getInternalFormat( createInfo.format )
				, get( createInfo.buffer )->getInternal()
				, get( createInfo.buffer )->getInternalOffset() + m_offset
				, m_range );
		}
		else
		{
			glLogCall( context
				, glTexBuffer
				, GL_BUFFER_TARGET_TEXTURE
				, getInternalFormat( createInfo.format )
				, get( createInfo.buffer )->getInternal() );
		}

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
