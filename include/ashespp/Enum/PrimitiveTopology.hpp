/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PrimitiveTopology_HPP___
#define ___AshesPP_PrimitiveTopology_HPP___
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
	inline std::string getName( VkPrimitiveTopology value )
	{
		switch ( value )
		{
		case VK_PRIMITIVE_TOPOLOGY_POINT_LIST:
			return "point_list";
		case VK_PRIMITIVE_TOPOLOGY_LINE_LIST:
			return "line_list";
		case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP:
			return "line_strip";
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
			return "triangle_list";
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
			return "triangle_strip";
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
			return "triangle_fan";
		case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY:
			return "line_list_adj";
		case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:
			return "line_strip_adj";
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:
			return "triangle_list_adj";
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY:
			return "triangle_strip_adj";
		case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST:
			return "patch_list";
		default:
			assert( false && "Unsupported VkPrimitiveTopology." );
			return "Unsupported VkPrimitiveTopology";
		}
	}
}

#endif
