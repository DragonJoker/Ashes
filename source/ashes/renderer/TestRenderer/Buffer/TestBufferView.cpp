#include "Buffer/TestBufferView.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BufferView::BufferView( VkDevice device
		, VkBufferViewCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}
}
