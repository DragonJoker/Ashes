/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlError
		: GLenum
	{
		GL_SUCCESS = 0,
		GL_ERROR_INVALID_ENUM = 0x0500,
		GL_ERROR_INVALID_VALUE = 0x0501,
		GL_ERROR_INVALID_OPERATION = 0x0502,
		GL_ERROR_STACK_OVERFLOW = 0x0503,
		GL_ERROR_STACK_UNDERFLOW = 0x0504,
		GL_ERROR_OUT_OF_MEMORY = 0x0505,
		GL_ERROR_INVALID_FRAMEBUFFER_OPERATION = 0x0506,
	};

	std::string getName( GlError value );
	inline std::string toString( GlError value ) { return getName( value ); }
}
