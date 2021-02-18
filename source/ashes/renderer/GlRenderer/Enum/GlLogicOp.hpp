/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

namespace ashes::gl
{
	enum GlLogicOp
	{
		GL_LOGIC_OP_CLEAR = 0x1500,
		GL_LOGIC_OP_AND = 0x1501,
		GL_LOGIC_OP_AND_REVERSE = 0x1502,
		GL_LOGIC_OP_COPY = 0x1503,
		GL_LOGIC_OP_AND_INVERTED = 0x1504,
		GL_LOGIC_OP_NOOP = 0x1505,
		GL_LOGIC_OP_XOR = 0x1506,
		GL_LOGIC_OP_OR = 0x1507,
		GL_LOGIC_OP_NOR = 0x1508,
		GL_LOGIC_OP_EQUIV = 0x1509,
		GL_LOGIC_OP_INVERT = 0x150A,
		GL_LOGIC_OP_OR_REVERSE = 0x150B,
		GL_LOGIC_OP_COPY_INVERTED = 0x150C,
		GL_LOGIC_OP_OR_INVERTED = 0x150D,
		GL_LOGIC_OP_NAND = 0x150E,
		GL_LOGIC_OP_SET = 0x150F,
	};
	std::string getName( GlLogicOp value );
	inline std::string toString( GlLogicOp value ) { return getName( value ); }
	/**
	*\brief
	*	Convertit un VkLogicOp en GlLogicOp.
	*\param[in] value
	*	Le VkLogicOp.
	*\return
	*	Le GlLogicOp.
	*/
	GlLogicOp convert( VkLogicOp const & value );
}
