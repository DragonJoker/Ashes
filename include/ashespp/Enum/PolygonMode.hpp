/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PolygonMode_HPP___
#define ___AshesPP_PolygonMode_HPP___
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
	inline std::string getName( VkPolygonMode value )
	{
		switch ( value )
		{
		case VK_POLYGON_MODE_FILL:
			return "fill";
		case VK_POLYGON_MODE_LINE:
			return "line";
		case VK_POLYGON_MODE_POINT:
			return "point";
		case VK_POLYGON_MODE_FILL_RECTANGLE_NV:
			return "fill_rectangle";
		default:
			assert( false && "Unsupported VkPolygonMode." );
			throw std::runtime_error{ "Unsupported VkPolygonMode" };
		}

		return 0;
	}
}

#endif
