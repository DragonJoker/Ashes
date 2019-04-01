#include "AshesRenderer/Buffer/BufferView.hpp"

#include "AshesRenderer/Buffer/Buffer.hpp"
#include "AshesRenderer/Core/Device.hpp"
#include "AshesRenderer/Core/Exception.hpp"
#include "AshesRenderer/Core/Instance.hpp"

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
		if ( !device.getInstance().getFeatures().hasTexBufferRange
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
