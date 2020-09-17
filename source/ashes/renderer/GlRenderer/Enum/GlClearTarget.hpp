/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlClearTarget
		: GLenum
	{
		GL_CLEAR_TARGET_COLOR = 0x1800,
		GL_CLEAR_TARGET_DEPTH = 0x1801,
		GL_CLEAR_TARGET_STENCIL = 0x1802,
		GL_CLEAR_TARGET_DEPTH_STENCIL = 0x84F9,
	};
	std::string getName( GlClearTarget value );
	inline std::string toString( GlClearTarget value ) { return getName( value ); }
}
