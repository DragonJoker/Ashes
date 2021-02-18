/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlBlendOp
		: GLenum
	{
		GL_BLEND_OP_ADD = 0x8006,
		GL_BLEND_OP_MIN = 0x8007,
		GL_BLEND_OP_MAX = 0x8008,
		GL_BLEND_OP_SUBTRACT = 0x800A,
		GL_BLEND_OP_REVERSE_SUBTRACT = 0x800B,
	};
	std::string getName( GlBlendOp value );
	inline std::string toString( GlBlendOp value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkBlendOp en GlBlendOp.
	*\param[in] value
	*	Le VkBlendOp.
	*\return
	*	Le GlBlendOp.
	*/
	GlBlendOp convert( VkBlendOp const & value );
}
