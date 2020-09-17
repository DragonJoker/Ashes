/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlBlendFactor
		: GLenum
	{
		GL_BLEND_FACTOR_ZERO = 0,
		GL_BLEND_FACTOR_ONE = 1,
		GL_BLEND_FACTOR_SRC_COLOR = 0x0300,
		GL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR = 0x0301,
		GL_BLEND_FACTOR_SRC_ALPHA = 0x0302,
		GL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA = 0x0303,
		GL_BLEND_FACTOR_DST_ALPHA = 0x0304,
		GL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA = 0x0305,
		GL_BLEND_FACTOR_DST_COLOR = 0x0306,
		GL_BLEND_FACTOR_ONE_MINUS_DST_COLOR = 0x0307,
		GL_BLEND_FACTOR_SRC_ALPHA_SATURATE = 0x0308,
		GL_BLEND_FACTOR_CONSTANT_COLOR = 0x8001,
		GL_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR = 0x8002,
		GL_BLEND_FACTOR_CONSTANT_ALPHA = 0x8003,
		GL_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
		GL_BLEND_FACTOR_SRC1_ALPHA = 0x8589,
		GL_BLEND_FACTOR_SRC1_COLOR = 0x88F9,
		GL_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR = 0x88FA,
		GL_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA = 0x88FB,
	};
	std::string getName( GlBlendFactor value );
	inline std::string toString( GlBlendFactor value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkBlendFactor en GlBlendFactor.
	*\param[in] value
	*	Le VkBlendFactor.
	*\return
	*	Le GlBlendFactor.
	*/
	GlBlendFactor convert( VkBlendFactor const & value );
}
