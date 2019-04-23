#include "AshesPP/Buffer/BufferView.hpp"

#include "AshesPP/Buffer/Buffer.hpp"
#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Core/Instance.hpp"

#include <common/Exception.hpp>

namespace ashes
{
	BufferView::BufferView( Device const & device
		, BufferBase const & buffer
		, VkFormat format
		, VkDeviceSize offset
		, VkDeviceSize range )
		: m_device{ device }
		, m_buffer{ buffer }
		, m_format{ format }
		, m_offset{ offset }
		, m_range{ range }
	{
		if ( !device.getInstance().getFeatures().hasTexBufferRange
			&& ( offset != 0 || range != buffer.getSize() ) )
		{
			throw Exception{ VkResult::VK_ERROR_FEATURE_NOT_PRESENT, "Buffer range" };
		}

		VkBufferViewCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
			nullptr,
			0u,
			buffer,                    // buffer
			format,                    // format
			offset,                    // offset
			range,                     // range
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateBufferView( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "BufferView creation" );
		registerObject( m_device, "BufferView", this );
	}

	BufferView::~BufferView()
	{
		unregisterObject( m_device, this );

		if ( m_internal )
		{
			m_device.vkDestroyBufferView( m_device
				, m_internal
				, nullptr );
		}
	}
}
