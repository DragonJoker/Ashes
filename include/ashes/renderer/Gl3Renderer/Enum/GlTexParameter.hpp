/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Enum/AccessFlag.hpp>

namespace gl_renderer
{
	enum GlTexParameter
	{
		GL_TEX_PARAMETER_VIEW_MIN_LEVEL = 0x82DB,
		GL_TEX_PARAMETER_VIEW_NUM_LEVELS = 0x82DC,
		GL_TEX_PARAMETER_VIEW_MIN_LAYER = 0x82DD,
		GL_TEX_PARAMETER_VIEW_NUM_LAYERS = 0x82DE,
		GL_TEX_PARAMETER_IMMUTABLE_LEVELS = 0x82DF,
		GL_TEX_PARAMETER_IMMUTABLE_FORMAT = 0x912F,
		GL_TEX_PARAMETER_MAX_LEVEL = 0x813D,
		GL_TEX_PARAMETER_LOD_BIAS = 0x8501,
		GL_TEX_PARAMETER_SWIZZLE_R = 0x8E42,
		GL_TEX_PARAMETER_SWIZZLE_G = 0x8E43,
		GL_TEX_PARAMETER_SWIZZLE_B = 0x8E44,
		GL_TEX_PARAMETER_SWIZZLE_A = 0x8E45,
	};
	inline std::string getName( GlTexParameter value )
	{
		switch ( value )
		{
		case GL_TEX_PARAMETER_VIEW_MIN_LEVEL:
			return "GL_TEXTURE_VIEW_MIN_LEVEL";

		case GL_TEX_PARAMETER_VIEW_NUM_LEVELS:
			return "GL_TEXTURE_VIEW_NUM_LEVELS";

		case GL_TEX_PARAMETER_VIEW_MIN_LAYER:
			return "GL_TEXTURE_VIEW_MIN_LAYER";

		case GL_TEX_PARAMETER_VIEW_NUM_LAYERS:
			return "GL_TEXTURE_VIEW_NUM_LAYERS";

		case GL_TEX_PARAMETER_IMMUTABLE_LEVELS:
			return "GL_TEXTURE_IMMUTABLE_LEVELS";

		case GL_TEX_PARAMETER_IMMUTABLE_FORMAT:
			return "GL_TEXTURE_IMMUTABLE_FORMAT";

		default:
			assert( false && "Unsupported GlTexParameter" );
			return "GlTexParameter_UNKNOWN";
		}

		return std::string{};
	}
}
