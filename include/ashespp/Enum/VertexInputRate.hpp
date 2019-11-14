/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_VertexInputRate_HPP___
#define ___AshesPP_VertexInputRate_HPP___
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
	inline std::string getName( VkVertexInputRate value )
	{
		switch ( value )
		{
		case VK_VERTEX_INPUT_RATE_VERTEX:
			return "vertex";
		case VK_VERTEX_INPUT_RATE_INSTANCE:
			return "instance";
		default:
			assert( false && "Unsupported VkVertexInputRate." );
			throw std::runtime_error{ "Unsupported VkVertexInputRate" };
		}

		return 0;
	}
}

#endif
