/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Enum/WrapMode.hpp>

namespace gl_renderer
{
	enum GlWrapMode
		: GLenum
	{
		GL_WRAP_MODE_REPEAT = 0x2901,
		GL_WRAP_MODE_MIRRORED_REPEAT = 0x8370,
		GL_WRAP_MODE_CLAMP_TO_EDGE = 0x812F,
		GL_WRAP_MODE_CLAMP_TO_BORDER = 0x812D,
		GL_WRAP_MODE_MIRROR_CLAMP_TO_EDGE = 0x8743,
	};
	std::string getName( GlWrapMode value );
	/**
	*\brief
	*	Convertit un ashes::WrapMode en GlWrapMode.
	*\param[in] mode
	*	Le ashes::WrapMode.
	*\return
	*	Le GlWrapMode.
	*/
	GlWrapMode convert( ashes::WrapMode const & mode );
}
