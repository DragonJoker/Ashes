/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/FrontFace.hpp>

namespace gl_renderer
{
	enum GlFrontFace
		: GLenum
	{
		GL_FRONT_FACE_CW = 0x0900,
		GL_FRONT_FACE_CCW = 0x0901,
	};
	std::string getName( GlFrontFace value );
	/**
	*\brief
	*	Convertit un ashes::FrontFace en GlFrontFace.
	*\param[in] value
	*	Le ashes::FrontFace.
	*\return
	*	Le GlFrontFace.
	*/
	GlFrontFace convert( ashes::FrontFace const & value );
}
