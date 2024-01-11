#include "ashespp/Buffer/BufferView.hpp"

#include "ashespp/Buffer/Buffer.hpp"
#include "ashespp/Core/Device.hpp"
#include "ashespp/Core/Instance.hpp"

#include <ashes/common/Exception.hpp>

namespace ashes
{
	BufferView::BufferView( Device const & device
		, BufferBase const & buffer
		, VkFormat format
		, VkDeviceSize offset
		, VkDeviceSize range )
		: BufferView{ device, "BufferView", buffer, format, offset, range }
	{
	}
	
	BufferView::BufferView( Device const & device
		, std::string const & debugName
		, BufferBase const & buffer
		, VkFormat format
		, VkDeviceSize offset
		, VkDeviceSize range )
		: VkObject{ debugName }
		, m_device{ device }
		, m_buffer{ buffer }
		, m_format{ format }
		, m_offset{ offset }
		, m_range{ range }
	{
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
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "BufferView creation" );
		registerObject( m_device, debugName, *this );
	}

	BufferView::~BufferView()noexcept
	{
		unregisterObject( m_device, *this );

		if ( m_internal )
		{
			m_device.vkDestroyBufferView( m_device
				, m_internal
				, m_device.getAllocationCallbacks() );
		}
	}
}
