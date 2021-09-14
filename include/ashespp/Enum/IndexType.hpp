/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_IndexType_HPP___
#define ___AshesPP_IndexType_HPP___
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
	inline std::string getName( VkIndexType value )
	{
		switch ( value )
		{
		case VK_INDEX_TYPE_UINT16:
			return "uint16";
		case VK_INDEX_TYPE_UINT32:
			return "uint32";
#ifdef VK_NV_ray_tracing
		case VK_INDEX_TYPE_NONE_NV:
			return "none";
#endif
		default:
			assert( false && "Unsupported VkIndexType." );
			return "Unsupported VkIndexType";
		}
	}
}

#endif
