#include "Buffer/VkBufferView.hpp"

#include "Buffer/VkBuffer.hpp"
#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	BufferView::BufferView( Device const & device
		, Buffer const & buffer
		, ashes::Format format
		, uint32_t offset
		, uint32_t range )
		: ashes::BufferView{ device, buffer, format, offset, range }
		, m_device{ device }
	{
		VkBufferViewCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
			nullptr,
			0u,
			buffer,                    // buffer
			convert( format ),         // format
			offset,                    // offset
			range,                     // range
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateBufferView( m_device
			, &createInfo
			, nullptr
			, &m_view );
		checkError( res, "BufferView creation" );
	}

	BufferView::~BufferView()
	{
		if ( m_view )
		{
			m_device.vkDestroyBufferView( m_device
				, m_view
				, nullptr );
		}
	}
}
