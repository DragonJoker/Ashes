/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SubpassDescriptionFlags_HPP___
#define ___AshesPP_SubpassDescriptionFlags_HPP___
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
	inline std::string getName( VkSubpassDescriptionFlagBits value )
	{
		switch ( value )
		{
		case VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX:
			return "per_view_attributes";
		case VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX:
			return "per_view_position_x_only";
		default:
			assert( false && "Unsupported VkSubpassDescriptionFlagBits." );
			return "Unsupported VkSubpassDescriptionFlagBits";
		}
	}
}

#endif
