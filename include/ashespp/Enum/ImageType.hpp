/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageType_HPP___
#define ___AshesPP_ImageType_HPP___
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
	inline std::string getName( VkImageType value )
	{
		switch ( value )
		{
		case VK_IMAGE_TYPE_1D:
			return "1d";
		case VK_IMAGE_TYPE_2D:
			return "2d";
		case VK_IMAGE_TYPE_3D:
			return "3d";
		default:
			assert( false && "Unsupported VkImageType." );
			throw std::runtime_error{ "Unsupported VkImageType" };
		}

		return 0;
	}
}

#endif
