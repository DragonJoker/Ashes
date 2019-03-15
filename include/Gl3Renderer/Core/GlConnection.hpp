/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Core/Connection.hpp>

namespace gl_renderer
{
	class Connection
		: public ashes::Connection
	{
	public:
		Connection( ashes::Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );

		bool getSurfaceSupport( uint32_t queueFamilyIndex )const override;
	};
}
