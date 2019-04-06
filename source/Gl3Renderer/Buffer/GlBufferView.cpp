#include "Buffer/GlBufferView.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"

#include <Ashes/Miscellaneous/Log.hpp>

namespace gl_renderer
{
	BufferView::BufferView( Device const & device
		, Buffer const & buffer
		, VkFormat format
		, uint32_t offset
		, uint32_t range )
		: ashes::BufferView{ device, buffer, format, offset, range }
		, m_device{ device }
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

		if ( device.getInstance().getFeatures().hasTexBufferRange )
		{
			glLogCall( context
				, glTexBufferRange_ARB
				, GL_BUFFER_TARGET_TEXTURE
				, getInternal( format )
				, buffer.getBuffer()
				, offset
				, range );
		}
		else
		{
			glLogCall( context
				, glTexBuffer
				, GL_BUFFER_TARGET_TEXTURE
				, getInternal( format )
				, buffer.getBuffer() );
		}

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
