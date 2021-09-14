/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_StencilOp_HPP___
#define ___AshesPP_StencilOp_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkStencilOp value )
	{
		switch ( value )
		{
		case VK_STENCIL_OP_KEEP:
			return "keep";
		case VK_STENCIL_OP_ZERO:
			return "zero";
		case VK_STENCIL_OP_REPLACE:
			return "replace";
		case VK_STENCIL_OP_INCREMENT_AND_CLAMP:
			return "incr_clamp";
		case VK_STENCIL_OP_DECREMENT_AND_CLAMP:
			return "decr_clamp";
		case VK_STENCIL_OP_INVERT:
			return "invert";
		case VK_STENCIL_OP_INCREMENT_AND_WRAP:
			return "incr_wrap";
		case VK_STENCIL_OP_DECREMENT_AND_WRAP:
			return "decr_wrap";
		default:
			assert( false && "Unsupported VkStencilOp." );
			return "Unsupported VkStencilOp";
		}
	}
}

#endif
