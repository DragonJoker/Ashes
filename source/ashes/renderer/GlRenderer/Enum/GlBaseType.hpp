/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlBaseType
	{
		GL_BYTE = 0x1400,
		GL_UNSIGNED_BYTE = 0x1401,
		GL_SHORT = 0x1402,
		GL_UNSIGNED_SHORT = 0x1403,
		GL_INT = 0x1404,
		GL_UNSIGNED_INT = 0x1405,
		GL_FLOAT = 0x1406,
	};
	std::string getName( GlBaseType value );
	inline std::string toString( GlBaseType value ) { return getName( value ); }
}
