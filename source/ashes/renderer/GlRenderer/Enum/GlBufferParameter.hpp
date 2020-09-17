/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlBufferParameter
		: GLenum
	{
		GL_BUFFER_PARAMETER_SIZE = 0x8764,
	};
	std::string getName( GlBufferParameter value );
	inline std::string toString( GlBufferParameter value ) { return getName( value ); }
}
