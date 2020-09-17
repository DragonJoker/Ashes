/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlShaderBinaryFormat
		: GLenum
	{
		GL_SHADER_BINARY_FORMAT_SPIR_V = 0x9551
	};

	inline std::string getName( GlShaderBinaryFormat value )
	{
		switch ( value )
		{
		case GL_SHADER_BINARY_FORMAT_SPIR_V:
			return "GL_SHADER_BINARY_FORMAT_SPIR_V";

		default:
			return "GL_SHADER_BINARY_FORMAT_UNKNOWN";
		}
	}
	inline std::string toString( GlShaderBinaryFormat value ) { return getName( value ); }
}
