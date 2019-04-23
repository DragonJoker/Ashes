/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/MemoryPropertyFlag.hpp>

namespace gl_renderer
{
	enum GlMemoryPropertyFlag
		: GLuint
	{
		GL_STREAM_DRAW = 0x88E0,
		GL_STREAM_READ = 0x88E1,
		GL_STREAM_COPY = 0x88E2,
		GL_STATIC_DRAW = 0x88E4,
		GL_STATIC_READ = 0x88E5,
		GL_STATIC_COPY = 0x88E6,
		GL_DYNAMIC_DRAW = 0x88E8,
		GL_DYNAMIC_READ = 0x88E9,
		GL_DYNAMIC_COPY = 0x88EA,
	};
	std::string getName( GlMemoryPropertyFlag value );
	/**
	*\brief
	*	Convertit un VkMemoryPropertyFlags en GlMemoryPropertyFlag.
	*\param[in] format
	*	Le VkMemoryPropertyFlags.
	*\return
	*	Le GlMemoryPropertyFlag.
	*/
	GlMemoryPropertyFlag convert( VkMemoryPropertyFlags const & flags );
}
