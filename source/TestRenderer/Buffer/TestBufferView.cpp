#include "Buffer/TestBufferView.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Core/TestDevice.hpp"

namespace test_renderer
{
	BufferView::BufferView( Device const & device
		, Buffer const & buffer
		, ashes::Format format
		, uint32_t offset
		, uint32_t range )
		: ashes::BufferView{ device, buffer, format, offset, range }
	{
	}

	BufferView::~BufferView()
	{
	}
}
