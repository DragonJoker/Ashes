/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlBackBuffer.hpp"

namespace gl_renderer
{
	BackBuffer::BackBuffer( ashes::Device const & device
		, ashes::TexturePtr && image
		, ashes::TextureViewPtr && view
		, uint32_t imageIndex )
		: ashes::BackBuffer{ device, std::move( image ), std::move( view ), imageIndex }
	{
	}
}
