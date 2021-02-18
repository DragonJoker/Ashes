/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#if defined( __APPLE__ )

#include <ashes/ashes.hpp>

#include "CGlContext.hpp"

namespace ashes::gl
{
	class RenderWindow
	{
	public:
		RenderWindow( int major
			, int minor
			, std::string const & name );
		~RenderWindow();
		VkMacOSSurfaceCreateInfoMVK getCreateInfo()const;

	private:
		CGLContextObj m_cglContext{ nullptr };
	};
}

#endif
