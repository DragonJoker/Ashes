#include "GlBufferView.hpp"

#include "GlBuffer.hpp"
#include "GlDevice.hpp"

namespace gl_renderer
{
	BufferView::BufferView( renderer::Device const & device
		, Buffer const & buffer
		, renderer::PixelFormat format
		, uint32_t offset
		, uint32_t range )
		: renderer::BufferView{ device, buffer, format, offset, range }
	{
	}

	BufferView::~BufferView()
	{
	}
}
