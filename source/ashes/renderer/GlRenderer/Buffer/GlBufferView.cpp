#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
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
			, &m_internal );
		glLogCall( context
			, glActiveTexture
			, GL_TEXTURE0 );
		glLogCall( context
			, glBindTexture
			, GL_BUFFER_TARGET_TEXTURE
			, m_internal );
		auto offset = get( createInfo.buffer )->getInternalOffset() + m_offset;

		if ( get( get( device )->getInstance() )->getFeatures().hasTexBufferRange )
		{
			glLogCall( context
				, glTexBufferRange
				, GL_BUFFER_TARGET_TEXTURE
				, getInternalFormat( m_format )
				, get( createInfo.buffer )->getInternal()
				, offset
				, m_range );
		}
		else
		{
			if ( ( offset > 0 || m_range != get( get( createInfo.buffer )->getMemory() )->getSize() ) )
			{
				get( m_device )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
					, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT
					, 0ull
					, 0u
					, VK_ERROR_VALIDATION_FAILED_EXT
					, "OpenGL"
					, "Texture buffer range is not supported" );
			}

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
			, &m_internal );
	}
}
