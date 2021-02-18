/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlStencilOp
	{
		GL_STENCIL_OP_ZERO = 0x0000,
		GL_STENCIL_OP_KEEP = 0x1E00,
		GL_STENCIL_OP_REPLACE = 0x1E01,
		GL_STENCIL_OP_INCR = 0x1E02,
		GL_STENCIL_OP_DECR = 0x1E03,
		GL_STENCIL_OP_INVERT = 0x150A,
		GL_STENCIL_OP_INCR_WRAP = 0x8507,
		GL_STENCIL_OP_DECR_WRAP = 0x8508,
	};
	std::string getName( GlStencilOp value );
	inline std::string toString( GlStencilOp value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkStencilOp en GlStencilOp.
	*\param[in] value
	*	Le VkStencilOp.
	*\return
	*	Le GlStencilOp.
	*/
	GlStencilOp convert( VkStencilOp const & value );
}
