#include "Buffer/BufferView.hpp"

#include "Buffer/Buffer.hpp"
#include "Core/Device.hpp"
#include "Core/Renderer.hpp"

namespace renderer
{
	BufferView::BufferView( Device const & device
		, BufferBase const & buffer
		, Format format
		, uint32_t offset
		, uint32_t range )
		: m_device{ device }
		, m_buffer{ buffer }
		, m_format{ format }
		, m_offset{ offset }
		, m_range{ range }
	{
		if ( !device.getRenderer().getFeatures().hasTexBufferRange
			&& ( offset != 0 || range != buffer.getSize() ) )
		{
			throw std::runtime_error( "Buffer range feature is not supported" );
		}

		registerObject( m_device, "BufferView", this );
	}

	BufferView::~BufferView()
	{
		unregisterObject( m_device, this );
	}
}
