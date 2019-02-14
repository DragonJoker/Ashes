/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/ImageTiling.hpp>

namespace gl_renderer
{
	enum GlImageTiling
	{
		GL_OPTIMAL_TILING_EXT = 0x9584,
		GL_LINEAR_TILING_EXT = 0x9585,
	};
	std::string getName( GlImageTiling value );
	/**
	*\brief
	*	Convertit un ashes::ImageTiling en GlImageTiling.
	*\param[in] value
	*	Le ashes::ImageTiling.
	*\return
	*	Le GlImageTiling.
	*/
	GlImageTiling convert( ashes::ImageTiling const & value );
}
