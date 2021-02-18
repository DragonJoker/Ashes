/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlFrontFace
		: GLenum
	{
		GL_FRONT_FACE_CW = 0x0900,
		GL_FRONT_FACE_CCW = 0x0901,
	};
	std::string getName( GlFrontFace value );
	inline std::string toString( GlFrontFace value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkFrontFace en GlFrontFace.
	*\param[in] value
	*	Le VkFrontFace.
	*\return
	*	Le GlFrontFace.
	*/
	GlFrontFace convert( VkFrontFace value );
}
