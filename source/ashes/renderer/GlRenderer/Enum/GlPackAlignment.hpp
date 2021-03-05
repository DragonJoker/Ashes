/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlPackAlignment
		: uint32_t
	{
		GL_UNPACK_ALIGNMENT = 0x0CF5,
		GL_PACK_ALIGNMENT = 0x0D05,
	};
	std::string getName( GlPackAlignment value );
	inline std::string toString( GlPackAlignment value ) { return getName( value ); }
}
