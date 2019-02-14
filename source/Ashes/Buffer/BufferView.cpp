#include "Ashes/Buffer/BufferView.hpp"

#include "Ashes/Buffer/Buffer.hpp"
#include "Ashes/Core/Device.hpp"
#include "Ashes/Core/Exception.hpp"
#include "Ashes/Core/Renderer.hpp"

namespace ashes
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
			throw Exception{ Result::eErrorFeatureNotPresent, "Buffer range" };
		}

		registerObject( m_device, "BufferView", this );
	}

	BufferView::~BufferView()
	{
		unregisterObject( m_device, this );
	}
}
