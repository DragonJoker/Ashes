/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/CullModeFlag.hpp>

namespace gl_renderer
{
	enum GlCullMode
		: GLenum
	{
		GL_CULL_MODE_NONE = 0x0000,
		GL_CULL_MODE_FRONT = 0x0404,
		GL_CULL_MODE_BACK = 0x0405,
		GL_CULL_MODE_FRONT_AND_BACK = 0x0408,
	};
	std::string getName( GlCullMode value );
	/**
	*\brief
	*	Convertit un ashes::CullModeFlags en GlCullMode.
	*\param[in] flags
	*	Le ashes::CullModeFlags.
	*\return
	*	Le GlCullMode.
	*/
	GlCullMode convert( ashes::CullModeFlags const & flags );
}
