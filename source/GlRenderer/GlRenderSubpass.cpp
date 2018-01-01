/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlRenderSubpass.hpp"

#include "GlDevice.hpp"
#include "GlRenderSubpassState.hpp"

namespace gl_renderer
{
	RenderSubpass::RenderSubpass( renderer::Device const & device
		, std::vector< utils::PixelFormat > const & formats
		, renderer::RenderSubpassState const & neededState )
		: renderer::RenderSubpass{ device, formats, neededState }
		, m_subPass{ static_cast< Device const & >( device ).getDevice()
			, convert< VkFormat >( formats )
			, convert( neededState ) }
	{
	}
}
